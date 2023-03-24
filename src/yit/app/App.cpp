#include "App.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

yit::App::App(int argc, char* argv[]) : args(argv + 1, argv + argc) {}

void yit::App::run() {
    po::options_description global_options("Global options");
    global_options.add_options()
        ("command", po::value<std::string>(), "command to execute")
        ("subargs", po::value<std::vector<std::string>>(), "arguments for command");
    
    po::positional_options_description global_positional;
    global_positional
        .add("command", 1)
        .add("subargs", -1);
    
    po::parsed_options parsed = po::command_line_parser(this->args)
        .options(global_options)
        .positional(global_positional)
        .allow_unregistered()
        .run();
    
    po::variables_map global_vm;
    po::store(parsed, global_vm);

    if (!global_vm.count("command")) {
        std::cout << "No subcommand was specified" << std::endl;
    } else if (global_vm["command"].as<std::string>() == "init") {
        std::vector<std::string> subargs = po::collect_unrecognized(parsed.options, po::include_positional);
        subargs.erase(subargs.begin());
        this->subcommand_init(subargs);
    } else {
        std::cout << "Unknown command: " << global_vm["command"].as<std::string>() << std::endl;
    }
}

void yit::App::subcommand_init(std::vector<std::string>& args) {
    po::options_description init_options("Initialization options");
    init_options.add_options()
        ("help", "produces this message")
        ("path,p", po::value<std::string>()->required()->value_name("path"), "path to the folder in which repository should be initialized");

    po::positional_options_description init_positional;
    init_positional
        .add("path", 1);

    po::parsed_options parsed_options = po::command_line_parser(args)
        .options(init_options)
        .positional(init_positional)
        .allow_unregistered()
        .run();

    po::variables_map init_vm;
    po::store(parsed_options, init_vm);
    
    if (init_vm.count("help")) {
        std::cout << init_options << std::endl;
    } else if (init_vm.count("path")) {
        std::cout << "initialized repo at path: " << init_vm["path"].as<std::string>() << std::endl;
    } else {
        std::cout << "No action was specified" << std::endl;
        std::cout << init_options << std::endl;
    }
}
