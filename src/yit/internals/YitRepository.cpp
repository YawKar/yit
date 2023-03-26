#include "YitRepository.hpp"
#include <fmt/core.h>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/fstream.hpp>

namespace yit::internals {

    YitRepository YitRepository::initialize(const fs::path& path) {
        YitRepository repo(path, true);
        if (fs::exists(repo.yit_dir)) {
            throw std::runtime_error(fmt::format(".yit folder already exists: {}", repo.yit_dir.string()));
        }
        repo.get_repo_dir({"branches"});
        repo.get_repo_dir({"objects"});
        repo.get_repo_dir({"refs", "tags"});
        repo.get_repo_dir({"refs", "heads"});

        fs::ofstream description(repo.get_repo_file({"description"}));
        description << "Unnamed repository; edit this file 'description' to name the repository.\n";

        fs::ofstream head(repo.get_repo_file({"HEAD"}));
        head << "ref: refs/heads/master\n";

        auto config = default_config();
        pt::write_ini(repo.get_repo_file({"config"}).string(), config);

        return repo;
    }

    pt::ptree YitRepository::default_config() {
        pt::ptree config;
        // yit only supports 0. https://git-scm.com/docs/repository-version
        config.put("core.rerepositoryformatversion", 0);
        // yit doesn't track the executable bit of files in the working tree. https://git-scm.com/docs/git-config#Documentation/git-config.txt-corefileMode
        config.put("core.filemode", false);
        // indicates that this repository has a worktree (thus not bare). https://git-scm.com/docs/git-config#Documentation/git-config.txt-corebare
        config.put("core.bare", false);
        return config;
    }
    
    YitRepository::YitRepository(const fs::path& path, bool force) : work_tree(path), yit_dir(path / ".yit") {
        if (!(fs::exists(yit_dir) && fs::is_directory(yit_dir) || force)) {
            throw std::runtime_error(fmt::format("Not a Yit repository: {}", work_tree.string()));
        }
        
        auto config_path = yit_dir / "config";
        if (fs::exists(config_path) && fs::is_regular_file(config_path)) {
            pt::read_ini(config_path.string(), config);
        } else if (!force) {
            throw std::runtime_error(fmt::format("Configuration file missing"));
        }
        
        if (!force) {
            int repository_format_version = config.get<int>("core.repositoryformatversion");
            if (repository_format_version != 0) {
                throw std::runtime_error(fmt::format("Unsupported core.repositoryformatversion: {}", repository_format_version));
            }
        }
    }

    fs::path YitRepository::get_repo_file(const std::initializer_list<fs::path> path) {
        if (path.size() == 0) {
            throw std::runtime_error(fmt::format("File is not specified"));
        }
        fs::path destination_dir_path;
        auto begin = path.begin();
        for (std::size_t i = 0; i < path.size() - 1; ++i) {
            destination_dir_path /= *begin;
            ++begin;
        }
        auto file = *begin;
        return get_repo_dir({destination_dir_path}) / file;
    }

    fs::path YitRepository::get_repo_dir(const std::initializer_list<fs::path> path) {
        auto repo_path = yit_dir;
        for (const auto& path_part : path) {
            repo_path /= path_part;
        }
        if (fs::exists(repo_path)) {
            if (fs::is_directory(repo_path)) {
                return repo_path;
            } else {
                throw std::runtime_error(fmt::format("Not a directory", repo_path.string()));
            }
        }
        fs::create_directories(repo_path);
        return repo_path;
    }

    fs::path YitRepository::get_work_tree() {
        return work_tree;
    }
}