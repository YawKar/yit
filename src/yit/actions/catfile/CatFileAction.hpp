#ifndef CAT_FILE_ACTION_HPP
#define CAT_FILE_ACTION_HPP
#include <boost/filesystem.hpp>
#include <string>

namespace fs = boost::filesystem;

namespace yit::actions {
class CatFileAction {
 public:
  static void cat_file(const std::string object_name, bool pretty_print);
};
}  // namespace yit::actions

#endif  // CAT_FILE_ACTION_HPP