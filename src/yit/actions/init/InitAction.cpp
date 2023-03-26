#include "InitAction.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fmt/core.h>
#include "../../internals/YitRepository.hpp"

namespace fs = boost::filesystem;

namespace yit::actions {

    void InitAction::init_repository(std::string path) {
        auto repo = yit::internals::YitRepository::initialize(path);
        fmt::print("Initialized empty yit repository in: {}\n", fs::canonical(repo.get_work_tree()).string());
    }
}