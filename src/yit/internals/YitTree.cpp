#include "YitTree.hpp"

namespace yit::internals {
YitTree::YitTree(std::vector<uint8_t>&& data) noexcept
    : YitObject(std::move(data)) {
  m_type = YitType::tree;
}

YitTree::YitTree(const std::vector<uint8_t>& data) noexcept : YitObject(data) {
  m_type = YitType::tree;
}

std::vector<uint8_t> YitTree::serialize() const { return m_data; }

void YitTree::deserialize(std::vector<uint8_t>&& data) {
  m_data = std::move(data);
}

void YitTree::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals