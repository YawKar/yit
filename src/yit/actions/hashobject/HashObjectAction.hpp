#ifndef HASH_OBJECT_ACTION_HPP
#define HASH_OBJECT_ACTION_HPP
#include <string>

namespace yit::actions {
class HashObjectAction {
 public:
  static void hash_object(const std::string file, const std::string type, bool write);
};
}  // namespace yit::actions

#endif  // HASH_OBJECT_ACTION_HPP