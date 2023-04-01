#ifndef YIT_BLOB_HPP
#define YIT_BLOB_HPP
#include <memory>
#include <vector>

#include "YitObject.hpp"

namespace yit::internals {

class YitBlob : public YitObject {
 public:
  YitBlob(std::vector<uint8_t>&& data) noexcept;

  YitBlob(const std::vector<uint8_t>& data) noexcept;

  std::vector<uint8_t> serialize() const override;

  void deserialize(std::vector<uint8_t>&& data) override;

  void deserialize(const std::vector<uint8_t>& data) override;
};

}  // namespace yit::internals
#endif  // YIT_BLOB_HPP