#include "YitBlob.hpp"

namespace yit::internals {
YitBlob::YitBlob(std::vector<uint8_t>&& data) noexcept : YitObject(std::move(data)) { m_type = YitType::blob; }

YitBlob::YitBlob(const std::vector<uint8_t>& data) noexcept : YitObject(data) { m_type = YitType::blob; }

std::vector<uint8_t> YitBlob::serialize() const { return m_data; }

void YitBlob::deserialize(std::vector<uint8_t>&& data) { m_data = std::move(data); }

void YitBlob::deserialize(const std::vector<uint8_t>& data) { m_data = data; }

}  // namespace yit::internals