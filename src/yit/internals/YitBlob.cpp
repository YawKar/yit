#include "YitBlob.hpp"

namespace yit::internals {

YitBlob::YitBlob(std::shared_ptr<char*> data) { this->blob_data = data; }

std::shared_ptr<char*> YitBlob::serialize() { return nullptr; }

void YitBlob::deserialize(std::shared_ptr<char*> data) { blob_data = data; }
}  // namespace yit::internals