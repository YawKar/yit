#include "HashObjectAction.hpp"

#include <fmt/core.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <zstr.hpp>

#include "../../internals/YitRepository.hpp"

namespace fs = boost::filesystem;

namespace yit::actions {

void HashObjectAction::hash_object(const std::string file, const std::string type, bool write) {
  auto repo_opt = yit::internals::YitRepository::lookup_for_root_repository();
  if (!repo_opt.has_value()) {
    throw std::runtime_error(fmt::format("Not a yit repository (or any parent up to the system root)"));
  }
  auto repo = repo_opt.value();

  fs::path file_path(file);
  if (!fs::exists(file_path)) {
    throw std::runtime_error(fmt::format("File doesn't exist: {}", file_path.string()));
  }

  std::vector<uint8_t> content;
  {
    std::ifstream input(file_path);
    char c;
    while (input.get(c) && !input.eof()) {
      content.push_back(c);
    }
    content.shrink_to_fit();
  }

  std::string digest;
  if (type == "blob") {
    digest = repo.write_object(internals::YitBlob(std::move(content)), write);
  } else if (type == "tree") {
    digest = repo.write_object(internals::YitTree(std::move(content)), write);
  } else if (type == "commit") {
    digest = repo.write_object(internals::YitCommit(std::move(content)), write);
  } else if (type == "tag") {
    digest = repo.write_object(internals::YitTag(std::move(content)), write);
  } else {
    throw std::runtime_error(fmt::format("Unknown type: {}", type));
  }
  fmt::print("{}\n", digest);
}
}  // namespace yit::actions
