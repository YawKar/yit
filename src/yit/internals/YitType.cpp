#include "YitType.hpp"

#include <stdexcept>

namespace yit::internals {
std::string to_string(const YitType type) {
  switch (type) {
    case YitType::unknown:
      return "unknown";
    case YitType::blob:
      return "blob";
    case YitType::tree:
      return "tree";
    case YitType::commit:
      return "commit";
    case YitType::tag:
      return "tag";
    default:
      throw std::runtime_error("Incorrect YitType");
  }
}
}  // namespace yit::internals