#include "YitTag.hpp"

namespace yit::internals {

YitTag::YitTag(std::shared_ptr<char*> data) { this->blob_data = data; }

std::shared_ptr<char*> YitTag::serialize() { return nullptr; }

void YitTag::deserialize(std::shared_ptr<char*> data) { blob_data = data; }
}  // namespace yit::internals