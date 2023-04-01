#include "CatFileAction.hpp"

#include <fmt/core.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <zstr.hpp>

#include "../../internals/YitRepository.hpp"

namespace fs = boost::filesystem;

namespace yit::actions {

void CatFileAction::cat_file(const std::string object_name, bool pretty_print) {
  auto repo_opt = yit::internals::YitRepository::lookup_for_root_repository();
  if (!repo_opt.has_value()) {
    throw std::runtime_error(fmt::format("Not a yit repository (or any parent up to the system root)"));
  }
  auto repo = repo_opt.value();
  auto object = repo.read_object(object_name);
  if (!pretty_print) {
    throw std::runtime_error("Currently, --pretty (-p) is required!");
  } else {
    if (std::holds_alternative<internals::YitBlob>(object)) {
      std::cout << std::get<internals::YitBlob>(object).serialize().data();
    }
  }
}
}  // namespace yit::actions
