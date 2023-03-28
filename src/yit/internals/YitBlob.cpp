#include "YitBlob.hpp"

namespace yit::internals {

YitBlob::YitBlob(std::shared_ptr<char*> data) {}

std::shared_ptr<char*> YitBlob::serialize() { return nullptr; }

void YitBlob::deserialize(std::shared_ptr<char*> data) {}
}  // namespace yit::internals