#include "YitTag.hpp"

namespace yit::internals {

std::vector<uint8_t> YitTag::serialize() const { return m_data; }

void YitTag::deserialize(std::vector<uint8_t>&& data) {
  m_data = std::move(data);
}

void YitTag::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals