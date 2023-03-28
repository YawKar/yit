#ifndef CAT_FILE_ACTION_HPP
#define CAT_FILE_ACTION_HPP
#include <boost/filesystem.hpp>
#include <string>

namespace fs = boost::filesystem;

namespace yit {
namespace actions {
class CatFileAction {
 public:
  static void cat_file(const std::string& object_name, bool pretty_print);

 private:
  static void pretty_print_object(const fs::path& object);
};
}  // namespace actions
}  // namespace yit

#endif  // CAT_FILE_ACTION_HPP