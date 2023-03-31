#include "YitCommit.hpp"

namespace yit::internals {

std::vector<uint8_t> YitCommit::serialize() const { return m_data; }

void YitCommit::deserialize(std::vector<uint8_t>&& data) {
  m_data = std::move(data);
}

void YitCommit::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals