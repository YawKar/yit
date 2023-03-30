#ifndef YIT_COMMIT_HPP
#define YIT_COMMIT_HPP
#include <memory>

namespace yit::internals {

class YitCommit {
 public:
  YitCommit(std::shared_ptr<char*> data);

  std::shared_ptr<char*> serialize();

  void deserialize(std::shared_ptr<char*> data);

 private:
  std::shared_ptr<char*> blob_data;
};

}  // namespace yit::internals

#endif  // YIT_COMMIT_HPP