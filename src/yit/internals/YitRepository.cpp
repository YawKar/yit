#include "YitRepository.hpp"

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <fmt/core.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <zstr.hpp>

namespace yit::internals {

YitRepository YitRepository::initialize(const fs::path& work_tree_path) {
  YitRepository repo(work_tree_path, true);
  if (fs::exists(repo.yit_dir)) {
    throw std::runtime_error(
        fmt::format(".yit folder already exists: {}", repo.yit_dir.string()));
  }
  repo.get_repo_dir({"branches"}, true);
  repo.get_repo_dir({"objects"}, true);
  repo.get_repo_dir({"refs", "tags"}, true);
  repo.get_repo_dir({"refs", "heads"}, true);

  fs::ofstream description(repo.get_repo_file({"description"}, true));
  description << "Unnamed repository; edit this file 'description' to name the "
                 "repository.\n";

  fs::ofstream head(repo.get_repo_file({"HEAD"}, true));
  head << "ref: refs/heads/master\n";

  auto config = default_config();
  pt::write_ini(repo.get_repo_file({"config"}, true).string(), config);

  return repo;
}

std::optional<YitRepository> YitRepository::lookup_for_root_repository() {
  auto cwd = fs::current_path();
  do {
    for (fs::directory_entry& sub : fs::directory_iterator(cwd)) {
      if (fs::is_directory(sub) && sub.path().filename() == ".yit") {
        return std::optional<YitRepository>(YitRepository(cwd, false));
      }
    }
    cwd = cwd.parent_path();
  } while (!cwd.empty());
  return std::optional<YitRepository>();
}

pt::ptree YitRepository::default_config() {
  pt::ptree config;
  // yit only supports 0. https://git-scm.com/docs/repository-version
  config.put("core.repositoryformatversion", 0);
  // yit doesn't track the executable bit of files in the working tree.
  // https://git-scm.com/docs/git-config#Documentation/git-config.txt-corefileMode
  config.put("core.filemode", false);
  // indicates that this repository has a worktree (thus not bare).
  // https://git-scm.com/docs/git-config#Documentation/git-config.txt-corebare
  config.put("core.bare", false);
  return config;
}

YitRepository::YitRepository(const fs::path& work_tree_path, bool force)
    : work_tree(work_tree_path), yit_dir(work_tree_path / ".yit") {
  if (!((fs::exists(yit_dir) && fs::is_directory(yit_dir)) || force)) {
    throw std::runtime_error(
        fmt::format("Not a Yit repository: {}", work_tree.string()));
  }

  auto config_path = yit_dir / "config";
  if (fs::exists(config_path) && fs::is_regular_file(config_path)) {
    pt::read_ini(config_path.string(), config);
  } else if (!force) {
    throw std::runtime_error(fmt::format("Configuration file missing"));
  }

  if (!force) {
    int repository_format_version =
        config.get<int>("core.repositoryformatversion");
    if (repository_format_version != 0) {
      throw std::runtime_error(
          fmt::format("Unsupported core.repositoryformatversion: {}",
                      repository_format_version));
    }
  }
}

fs::path YitRepository::get_repo_file(
    const std::initializer_list<fs::path> path, bool mkdir) {
  if (path.size() == 0) {
    throw std::runtime_error(fmt::format("File is not specified"));
  }
  fs::path destination_dir_path;
  auto begin = path.begin();
  for (std::size_t i = 0; i < path.size() - 1; ++i) {
    destination_dir_path /= *begin;
    ++begin;
  }
  auto file = *begin;
  return get_repo_dir({destination_dir_path}, mkdir) / file;
}

fs::path YitRepository::get_repo_dir(const std::initializer_list<fs::path> path,
                                     bool mkdir) {
  auto repo_path = yit_dir;
  for (const auto& path_part : path) {
    repo_path /= path_part;
  }
  if (fs::exists(repo_path)) {
    if (fs::is_directory(repo_path)) {
      return repo_path;
    } else {
      throw std::runtime_error(
          fmt::format("Not a directory", repo_path.string()));
    }
  }
  if (mkdir) {
    fs::create_directories(repo_path);
  }
  return repo_path;
}

bool YitRepository::is_valid_sha(const std::string& sha) {
  return sha.length() == 40 && std::all_of(sha.begin(), sha.end(), [](char s) {
           return std::isxdigit(s);
         });
}

std::variant<YitBlob, YitTree, YitCommit, YitTag> YitRepository::read_object(
    const std::string sha) {
  if (!is_valid_sha(sha)) {
    throw std::runtime_error(fmt::format("Not a valid SHA-1 digest: {}", sha));
  }
  std::string lowercased_sha = sha;
  std::transform(lowercased_sha.begin(), lowercased_sha.end(),
                 lowercased_sha.begin(), ::tolower);
  auto object_path = get_repo_file(
      {"objects", lowercased_sha.substr(0, 2), lowercased_sha.substr(2)},
      false);
  if (!fs::exists(object_path)) {
    throw std::runtime_error(
        fmt::format("Object not found: {}", lowercased_sha));
  }
  zstr::ifstream object_file(object_path.string());
  object_file.unsetf(std::ios_base::skipws);
  std::string type;
  object_file >> type;
  if (!(type == "tree" || type == "blob" || type == "commit" ||
        type == "tag")) {
    throw std::runtime_error(fmt::format(
        "Malformed object {}: unknown type: {}", lowercased_sha, type));
  }
  uint32_t size = 0;
  {
    std::string size_str;
    char c;
    while (object_file.get(c) && c != 0) {
      size_str += c;
    }
    size = std::stoi(size_str);
  }
  std::vector<uint8_t> content;
  while (!object_file.eof()) {
    content.push_back(static_cast<char>(object_file.get()));
  }
  content.pop_back();
  content.shrink_to_fit();
  if (size != content.size()) {
    throw std::runtime_error(
        fmt::format("Malformed object {}: bad size (written: {}, real: {})",
                    lowercased_sha, size, content.size()));
  }
  if (type == "blob") {
    return YitBlob(std::move(content));
  } else if (type == "commit") {
    return YitCommit(std::move(content));
  } else if (type == "tree") {
    return YitTree(std::move(content));
  } else if (type == "tag") {
    return YitTag(std::move(content));
  } else {
    throw std::runtime_error(fmt::format(
        "Malformed object {}: unknown type: {}", lowercased_sha, type));
  }
}

std::string YitRepository::write_object(const YitObject& object, bool write) {
  if (object.get_type() == YitType::unknown) {
    throw std::runtime_error("Yit object's type is unknown");
  }
  auto type = internals::to_string(object.get_type());
  auto data = object.serialize();
  std::string result = type + ' ' + std::to_string(data.size()) + '\x00';
  result.reserve(result.size() + data.size());
  for (const uint8_t c : data) {
    result.push_back(c);
  }
  CryptoPP::SHA1 hash;
  hash.Update(data.data(), data.size());
  std::string digest;
  digest.resize(hash.DigestSize());
  hash.Final(reinterpret_cast<uint8_t*>(&digest[0]));
  std::string hex_digest;
  hex_digest.reserve(40);
  auto tohex = [](const char c) -> char {
    static std::string alphabet = "01234567890ABCDEF";
    return alphabet.at(c);
  };
  for (const char c : digest) {
    hex_digest.push_back(tohex(c >> 4));
    hex_digest.push_back(tohex(c & 0xF));
  }
  if (write) {
    auto file_path = get_repo_file(
        {"objects", hex_digest.substr(0, 2), hex_digest.substr(2)}, true);
    zstr::ofstream outstream(file_path.string());
    outstream << result;
  }
  return hex_digest;
}

fs::path YitRepository::get_work_tree() { return work_tree; }
}  // namespace yit::internals