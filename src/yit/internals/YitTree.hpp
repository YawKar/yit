#ifndef YIT_TREE_HPP
#define YIT_TREE_HPP
#include <memory>

namespace yit::internals {

class YitTree {
 public:
  YitTree(std::shared_ptr<char*> data);

  std::shared_ptr<char*> serialize();

  void deserialize(std::shared_ptr<char*> data);

 private:
  std::shared_ptr<char*> blob_data;
};

}  // namespace yit::internals

#endif  // YIT_TREE_HPP