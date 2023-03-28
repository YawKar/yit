#ifndef YIT_BLOB_HPP
#define YIT_BLOB_HPP
#include <memory>

namespace yit::internals {

class YitBlob {
 public:
  YitBlob(std::shared_ptr<char*> data);

  std::shared_ptr<char*> serialize();

  void deserialize(std::shared_ptr<char*> data);

 private:
  std::shared_ptr<char*> blob_data;
};

}  // namespace yit::internals
#endif  // YIT_BLOB_HPP