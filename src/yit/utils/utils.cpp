#include "utils.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace yit {
    namespace utils {
        std::optional<fs::path> find_yit_folder() {
            auto cwd = fs::current_path();
            do {
                for (fs::directory_entry& sub : fs::directory_iterator(cwd)) { 
                    if (is_yit_folder(sub)) {
                        return std::optional<fs::path>(sub);
                    }
                }
                cwd = cwd.parent_path();
            } while (!cwd.empty());
            return std::optional<fs::path>();
        }

        bool is_yit_folder(const fs::path& path) {
            return fs::is_directory(path) && path.filename() == ".yit";
        }
    }
}