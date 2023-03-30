#include "YitCommit.hpp"

namespace yit::internals {

YitCommit::YitCommit(std::shared_ptr<char*> data) { this->blob_data = data; }

std::shared_ptr<char*> YitCommit::serialize() { return nullptr; }

void YitCommit::deserialize(std::shared_ptr<char*> data) { blob_data = data; }
}  // namespace yit::internals