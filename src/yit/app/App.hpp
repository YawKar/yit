#ifndef YIT_APP_HPP
#define YIT_APP_HPP
#include <vector>
#include <string>

namespace yit {
    class App {
    public:
        std::vector<std::string> args;

        App(int argc, char* argv[]);
        
        void run();
    private:
        void subcommand_init(std::vector<std::string>& args);
    };
}

#endif // YIT_APP_HPP
