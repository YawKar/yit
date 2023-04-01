#include "YitObject.hpp"

namespace yit::internals {
YitObject::YitObject(std::vector<uint8_t>&& data) noexcept
    : m_data(std::move(data)), m_type(YitType::unknown) {}

YitObject::YitObject(const std::vector<uint8_t>& data) noexcept
    : m_data(data), m_type(YitType::unknown) {}

YitType YitObject::get_type() const { return m_type; }
}  // namespace yit::internals