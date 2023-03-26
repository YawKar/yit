#ifndef YIT_REPOSITORY_HPP
#define YIT_REPOSITORY_HPP
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace yit::internals {

    /// @brief Representation of a yit repository that aggregates main paths and config.
    class YitRepository {
    public:
        /**
         * @brief Create a new repository inside the given path. Create directories if not exist.
         *        Panic if `.yit/` directory already exists.
         * @param path path to the target directory.
         * @return YitRepository instance that represents the new repository.
        */
        static YitRepository initialize(const fs::path& path);

        /**
         * @return Default config for fresh yit repository.
        */
        static pt::ptree default_config();

        /**
         * @brief Create a yit repository representation from the given path.
         *        Panic if given path or the yit repository inside of it is corrupted.
         *        Don't panic if force is true.
         * @param path path to the directory.
         * @param force if true, don't panic.
        */
        YitRepository(const fs::path& path, bool force);

        /**
         * @brief Create directories (if not exist) inside `.yit/` directory according to the given relative path.
         *        If `path.size()` is 0, return path to the `./yit` directory.
         * @param path relative to `.yit/`.
         * @return path to the target directory.
        */
        fs::path get_repo_dir(const std::initializer_list<fs::path> path);

        /**
         * @brief Create directories (if not exist) inside .yit/ directory according to the given relative path.
         *        Do not create file. Panic, if `path.size()` is 0.
         * @param path relative to `.yit/`.
         * @return path to the target file.
        */
        fs::path get_repo_file(const std::initializer_list<fs::path> path);

        /**
         * @return path to the working tree directory
        */
        fs::path get_work_tree();
    private:
        /// @brief Working directory of the repository.
        fs::path work_tree;
        /// @brief .yit directory of the repository.
        fs::path yit_dir;
        /// @brief Configuration properties of the repository.
        pt::ptree config;
    };
}

#endif // YIT_REPOSITORY_HPP
