#ifndef YIT_OBJECT_HPP
#define YIT_OBJECT_HPP
#include <cstdint>
#include <string>
#include <vector>

#include "YitType.hpp"

namespace yit::internals {

class YitObject {
 public:
  YitObject(std::vector<uint8_t>&& data) noexcept;

  YitObject(const std::vector<uint8_t>& data) noexcept;

  virtual std::vector<uint8_t> serialize() const = 0;

  virtual void deserialize(std::vector<uint8_t>&& data) = 0;

  virtual void deserialize(const std::vector<uint8_t>& data) = 0;

  YitType get_type() const;

 protected:
  std::vector<uint8_t> m_data;
  YitType m_type;
};

}  // namespace yit::internals

#endif  // YIT_OBJECT_HPP