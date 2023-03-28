#ifndef INIT_ACTION_HPP
#define INIT_ACTION_HPP
#include <string>

namespace yit {
namespace actions {
class InitAction {
 public:
  static void init_repository(std::string path);
};
}  // namespace actions
}  // namespace yit

#endif  // INIT_ACTION_HPP