#ifndef YIT_TAG_HPP
#define YIT_TAG_HPP
#include <memory>

namespace yit::internals {

class YitTag {
 public:
  YitTag(std::shared_ptr<char*> data);

  std::shared_ptr<char*> serialize();

  void deserialize(std::shared_ptr<char*> data);

 private:
  std::shared_ptr<char*> blob_data;
};

}  // namespace yit::internals

#endif  // YIT_TAG_HPP