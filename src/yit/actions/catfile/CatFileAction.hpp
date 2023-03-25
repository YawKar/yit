#ifndef CAT_FILE_ACTION_HPP
#define CAT_FILE_ACTION_HPP
#include <string>

namespace yit {
    namespace actions {
        class CatFileAction {
        public:
            static void cat_file(std::string object_name, bool pretty_print);
        };
    }
}

#endif // CAT_FILE_ACTION_HPP