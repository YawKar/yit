#include "YitBlob.hpp"

namespace yit::internals {

std::vector<uint8_t> YitBlob::serialize() const { return m_data; }

void YitBlob::deserialize(std::vector<uint8_t>&& data) {
  m_data = std::move(data);
}

void YitBlob::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals