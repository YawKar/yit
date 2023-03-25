#ifndef YIT_UTILS_HPP
#define YIT_UTILS_HPP
#include <boost/filesystem.hpp>
#include <optional>

namespace fs = boost::filesystem;

namespace yit {
    namespace utils {
        extern std::optional<fs::path> find_yit_folder();
        extern bool is_yit_folder(const fs::path& path);
    }
}

#endif // YIT_UTILS_HPP