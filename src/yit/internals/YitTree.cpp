#include "YitTree.hpp"

namespace yit::internals {

std::vector<uint8_t> YitTree::serialize() const { return m_data; }

void YitTree::deserialize(std::vector<uint8_t>&& data) {
  m_data = std::move(data);
}

void YitTree::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals