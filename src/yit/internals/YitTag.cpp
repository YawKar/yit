#include "YitTag.hpp"

namespace yit::internals {
YitTag::YitTag(std::vector<uint8_t>&& data) noexcept : YitObject(std::move(data)) { m_type = YitType::tag; }

YitTag::YitTag(const std::vector<uint8_t>& data) noexcept : YitObject(data) { m_type = YitType::tag; }

std::vector<uint8_t> YitTag::serialize() const { return m_data; }

void YitTag::deserialize(std::vector<uint8_t>&& data) { m_data = std::move(data); }

void YitTag::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals