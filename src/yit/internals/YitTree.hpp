#ifndef YIT_TREE_HPP
#define YIT_TREE_HPP
#include <memory>

#include "YitObject.hpp"

namespace yit::internals {

class YitTree : public YitObject {
 public:
  YitTree(std::vector<uint8_t>&& data) noexcept;

  YitTree(const std::vector<uint8_t>& data) noexcept;

  std::vector<uint8_t> serialize() const override;

  void deserialize(std::vector<uint8_t>&& data) override;

  void deserialize(const std::vector<uint8_t>& data) override;
};

}  // namespace yit::internals

#endif  // YIT_TREE_HPP