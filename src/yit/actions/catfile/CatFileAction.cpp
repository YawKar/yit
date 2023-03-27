#include "CatFileAction.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fmt/core.h>
#include <zstr.hpp>
#include "../../internals/YitRepository.hpp"

namespace fs = boost::filesystem;

namespace yit::actions {

    void CatFileAction::cat_file(const std::string& object_name, bool pretty_print) {
        if (object_name.length() != 40) {
            throw std::runtime_error(fmt::format("Object name should be 40 characters long but was {}", object_name.length()));
        }
        auto repo_opt = yit::internals::YitRepository::lookup_for_root_repository();
        if (!repo_opt.has_value()) {
            throw std::runtime_error(fmt::format("Not a yit repository (or any parent up to the system root)"));
        }
        auto repo = repo_opt.value();
        auto object = repo.get_repo_file({"objects", object_name.substr(0, 2), object_name.substr(2)}, false);
        if (!fs::exists(object)) {
            throw std::runtime_error(fmt::format("Object was not found, name: {}", object_name));
        }
        if (!pretty_print) {
            throw std::runtime_error("Currently, --pretty (-p) is required!");
        } else {
            pretty_print_object(object);
        }
    }

    void CatFileAction::pretty_print_object(const fs::path& object) {
        zstr::ifstream object_stream(object.string());
        char s;
        while (object_stream >> std::noskipws >> s) {
            fmt::print("{}\n", s);
        }
    }
}
