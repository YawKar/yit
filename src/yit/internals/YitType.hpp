#ifndef YIT_TYPE_HPP
#define YIT_TYPE_HPP
#include <string>

namespace yit::internals {
enum class YitType { unknown, blob, tree, commit, tag };

extern std::string to_string(const YitType type);
}  // namespace yit::internals

#endif  // YIT_TYPE_HPP