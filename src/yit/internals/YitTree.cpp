#include "YitTree.hpp"

namespace yit::internals {

YitTree::YitTree(std::shared_ptr<char*> data) { this->blob_data = data; }

std::shared_ptr<char*> YitTree::serialize() { return nullptr; }

void YitTree::deserialize(std::shared_ptr<char*> data) { blob_data = data; }
}  // namespace yit::internals