#include "InitAction.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

namespace fs = boost::filesystem;

namespace yit {
    namespace actions {

        void InitAction::init_repository(std::string path) {
            fs::path path_obj(path);
            if (fs::is_regular_file(path)) {
                throw std::runtime_error("Couldn't initialize repository because path is a regular file");
                return;
            }
            if (!fs::exists(path_obj)) {
                std::cout << "Warning: path didn't exist. Yit creates it." << std::endl;
                fs::create_directories(path_obj);
            }
            auto yit_folder = path_obj / ".yit";
            if (fs::exists(yit_folder)) {
                throw std::runtime_error(".yit folder already exists. If you want to reinitialize repository, perhaps, delete the existing .yit folder.");
            }
            fs::create_directory(yit_folder);
            auto objects_folder = yit_folder / "objects";
            fs::create_directory(objects_folder);
            auto refs_folder = yit_folder / "refs";
            fs::create_directory(refs_folder);
            auto head_ref_file = yit_folder / "HEAD";
            fs::ofstream head_ref(head_ref_file);
            head_ref << "ref: refs/heads/master\n";

            std::cout << "Initialized empty Yit repository in " << yit_folder << std::endl;
        }
    }
}