#ifndef YIT_APP_HPP
#define YIT_APP_HPP
#include <string>
#include <vector>

namespace yit {
class App {
 public:
  void run(int argc, char* argv[]);

 private:
  void subcommand_init(std::vector<std::string>& args);
  void subcommand_cat_file(std::vector<std::string>& args);
  void subcommand_hash_object(std::vector<std::string>& args);
  bool does_contain_general_help(std::vector<std::string>& args);
  void print_warning_about_unrecognized_options(
      std::vector<std::string>& unrecognized);
};
}  // namespace yit

#endif  // YIT_APP_HPP
