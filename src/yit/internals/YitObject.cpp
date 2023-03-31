#include "YitObject.hpp"

namespace yit::internals {
YitObject::YitObject(std::vector<uint8_t>&& data) noexcept
    : m_data(std::move(data)) {}

YitObject::YitObject(const std::vector<uint8_t>& data) noexcept
    : m_data(data) {}
}  // namespace yit::internals