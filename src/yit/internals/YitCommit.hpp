#ifndef YIT_COMMIT_HPP
#define YIT_COMMIT_HPP
#include <memory>

#include "YitObject.hpp"

namespace yit::internals {

class YitCommit : public YitObject {
 public:
  using YitObject::YitObject;

  std::vector<uint8_t> serialize() const override;

  void deserialize(std::vector<uint8_t>&& data) override;

  void deserialize(const std::vector<uint8_t>& data) override;
};

}  // namespace yit::internals

#endif  // YIT_COMMIT_HPP