#ifndef YIT_REPOSITORY_HPP
#define YIT_REPOSITORY_HPP
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <optional>
#include <variant>

#include "YitBlob.hpp"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace yit::internals {

/// @brief Representation of a yit repository that aggregates main paths and
/// config.
class YitRepository {
 public:
  /**
   * @brief Create a new repository inside the given path. Create directories if
   * not exist. Panic if `.yit/` directory already exists.
   * @param path path to the target directory.
   * @return YitRepository instance that represents the new repository.
   */
  static YitRepository initialize(const fs::path& path);

  /**
   * @brief Search for the root of the repository. Essentially, it tries to find
   * the nearest .yit directory among children of all parent directories of the
   * `current_path()`, includint `current_path()` itself.
   * @return the actual YitRepository instance or nothing if there's no yit
   * repository
   */
  static std::optional<YitRepository> lookup_for_root_repository();

  /**
   * @return Default config for fresh yit repository.
   */
  static pt::ptree default_config();

  /**
   * @brief Check whether the given `sha` string is indeed a valid SHA-1 digest.
   */
  static bool is_valid_sha(const std::string& sha);

  /**
   * @brief Create a yit repository representation from the given path.
   *        Panic if given path or the yit repository inside of it is corrupted.
   *        Don't panic if force is true.
   * @param path path to the directory.
   * @param force if true, don't panic.
   */
  YitRepository(const fs::path& path, bool force);

  /**
   * @brief Generate path to the target directory under `.yit` directory.
   *        If `path.size()` is 0, return path to the `./yit` directory.
   *        If `mkdir` is `true`, create directories (if not exist) inside
   * `.yit/` directory according to the given relative path.
   * @param path relative to `.yit/`.
   * @return path to the target directory.
   */
  fs::path get_repo_dir(const std::initializer_list<fs::path> path, bool mkdir);

  /**
   * @brief Generate path to the target file under `.yit` directory.
   *        If `mkdir` is `true`, create directories (if not exist) inside .yit/
   * directory according to the given relative path. Do not create file. Panic,
   * if `path.size()` is 0.
   * @param path relative to `.yit/`.
   * @return path to the target file.
   */
  fs::path get_repo_file(const std::initializer_list<fs::path> path,
                         bool mkdir);

  std::variant<YitBlob> read_object(const std::string sha);

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
}  // namespace yit::internals

#endif  // YIT_REPOSITORY_HPP
