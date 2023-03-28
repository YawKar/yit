#include "YitRepository.hpp"

#include <fmt/core.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ini_parser.hpp>
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
  if (!(fs::exists(yit_dir) && fs::is_directory(yit_dir) || force)) {
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

std::variant<YitBlob> YitRepository::read_object(const std::string& sha) {
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
  zstr::ifstream object_file(object_path.string(), std::ios_base::ate);
  object_file.unsetf(std::ios_base::skipws);
  std::string type;
  object_file >> type;
  if (!(type == "tree" || type == "blob" || type == "commit" ||
        type == "tag")) {
    throw std::runtime_error(fmt::format(
        "Malformed object {}: unknown type: {}", lowercased_sha, type));
  }
  uint size = 0;
  {
    std::string size_str;
    char c;
    while (object_file.get(c) && c != 0) {
      size_str += c;
    }
    size = std::stoi(size_str);
  }
}

fs::path YitRepository::get_work_tree() { return work_tree; }
}  // namespace yit::internals