#include "CatFileAction.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fmt/core.h>
#include <zstr.hpp>
#include "../../utils/utils.hpp"

namespace fs = boost::filesystem;

namespace yit {
    namespace actions {

        void CatFileAction::cat_file(const std::string& object_name, bool pretty_print) {
            if (object_name.length() != 40) {
                throw std::runtime_error(fmt::format("Object name should be 40 characters long but was {}", object_name.length()));
            }
            fs::path yit_folder;
            {
                auto yit_folder_opt = utils::find_yit_folder();
                if (!yit_folder_opt.has_value()) {
                    throw std::runtime_error(".yit folder was not found!");
                }
                yit_folder = yit_folder_opt.value();
            }
            auto objects_folder = yit_folder / "objects";
            if (!fs::exists(objects_folder)) {
                throw std::runtime_error(fmt::format("'objects' folder was not found in {}", yit_folder.string()));
            }
            auto objects_subfolder = objects_folder / object_name.substr(0, 2);
            if (!fs::exists(objects_subfolder)) {
                throw std::runtime_error(fmt::format("Object with name {} was not found", object_name));
            }
            auto object = objects_subfolder / object_name.substr(2);
            if (!fs::exists(object)) {
                throw std::runtime_error(fmt::format("Object with name {} was not found", object_name));
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
}
