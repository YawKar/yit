#include "CatFileAction.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include "../../utils/utils.hpp"

namespace fs = boost::filesystem;

namespace yit {
    namespace actions {

        void CatFileAction::cat_file(std::string object_name, bool pretty_print) {
            fs::path yit_folder;
            {
                auto yit_folder_opt = utils::find_yit_folder();
                if (!yit_folder_opt.has_value()) {
                    throw std::runtime_error(".yit folder was not found!");
                }
                yit_folder = yit_folder_opt.value();
            }
            std::cout << yit_folder << std::endl;
        }
    }
}
