#include "App.hpp"

#include <boost/program_options.hpp>
#include <iostream>

#include "../actions/actions.hpp"

namespace po = boost::program_options;

namespace yit {

void App::run(int _argc, char* _argv[]) {
  std::vector<std::string> args(_argv + 1, _argv + _argc);
  po::options_description global_options("Global options");
  global_options.add_options()("command", po::value<std::string>(),
                               "command to execute")(
      "subargs", po::value<std::vector<std::string>>(),
      "arguments for command");

  po::positional_options_description global_positional;
  global_positional.add("command", 1).add("subargs", -1);

  po::parsed_options parsed = po::command_line_parser(args)
                                  .options(global_options)
                                  .positional(global_positional)
                                  .allow_unregistered()
                                  .run();

  po::variables_map global_vm;
  po::store(parsed, global_vm);

  if (!global_vm.count("command")) {
    if (does_contain_general_help(args)) {
      po::options_description help_option("General options");
      help_option.add_options()("help", "produce this help message");
      std::cout << help_option << std::endl;
      std::cout << "Initialization subcommands:" << std::endl;
      std::cout << "\tinit"
                << "\t\tCreate an empty Yit repository" << std::endl;
      std::cout << "\tclone"
                << "\t\tClone a repository into a new directory" << std::endl;
      std::cout << "Object database subcommands:" << std::endl;
      std::cout << "\tcat-file"
                << "\tProvide content or type and size information for "
                   "repository objects"
                << std::endl;
    } else {
      std::cout << "ERROR! Subcommand not specified" << std::endl;
    }
  } else if (global_vm["command"].as<std::string>() == "init") {
    std::vector<std::string> subargs =
        po::collect_unrecognized(parsed.options, po::include_positional);
    subargs.erase(subargs.begin());
    this->subcommand_init(subargs);
  } else if (global_vm["command"].as<std::string>() == "cat-file") {
    std::vector<std::string> subargs =
        po::collect_unrecognized(parsed.options, po::include_positional);
    subargs.erase(subargs.begin());
    this->subcommand_cat_file(subargs);
  } else {
    std::cout << "ERROR! Unknown command: "
              << global_vm["command"].as<std::string>() << std::endl;
  }
}

/// @brief This method is solely designed to be used in non-terminal
/// subcommands.
///        If command is terminal, you should implement `--help` on your own.
/// @param args arguments to look for `--help`
/// @return whether args contain `--help`
bool App::does_contain_general_help(std::vector<std::string>& args) {
  po::options_description help_option("Generic help option");
  help_option.add_options()("help", "produce help message");

  po::parsed_options parsed = po::command_line_parser(args)
                                  .options(help_option)
                                  .allow_unregistered()
                                  .run();

  po::variables_map help_vm;
  po::store(parsed, help_vm);
  return help_vm.count("help");
}

void App::subcommand_init(std::vector<std::string>& args) {
  po::options_description init_options("init options");
  init_options.add_options()("help", "produce help message")(
      "path,p", po::value<std::string>()->required()->value_name("path"),
      "path to the folder in which repository should be initialized "
      "(positional argument)");

  po::positional_options_description init_positional;
  init_positional.add("path", 1);

  po::parsed_options parsed_options = po::command_line_parser(args)
                                          .options(init_options)
                                          .positional(init_positional)
                                          .allow_unregistered()
                                          .run();

  po::variables_map init_vm;
  po::store(parsed_options, init_vm);

  auto unrecognized =
      po::collect_unrecognized(parsed_options.options, po::exclude_positional);
  print_warning_about_unrecognized_options(unrecognized);

  if (init_vm.count("help")) {
    std::cout << init_options << std::endl;
  } else if (!init_vm.count("path")) {
    std::cout << "ERROR! Path was not specified (--path, -p)" << std::endl;
  } else {
    try {
      actions::InitAction::init_repository(init_vm["path"].as<std::string>());
    } catch (std::runtime_error e) {
      std::cout << "ERROR! " << e.what() << std::endl;
    }
  }
}

void App::subcommand_cat_file(std::vector<std::string>& args) {
  po::options_description cat_file_options("cat-file options");
  cat_file_options.add_options()("help", "produce help message")(
      "object", po::value<std::string>()->required()->value_name("SHA-1"),
      "the SHA-1 checksum name of the object to show (positional argument)")(
      "pretty,p", "pretty-print the contents of <object> based on its type");

  po::positional_options_description cat_file_positional;
  cat_file_positional.add("object", 1);

  po::parsed_options parsed_options = po::command_line_parser(args)
                                          .options(cat_file_options)
                                          .positional(cat_file_positional)
                                          .allow_unregistered()
                                          .run();

  po::variables_map cat_file_vm;
  po::store(parsed_options, cat_file_vm);

  auto unrecognized =
      po::collect_unrecognized(parsed_options.options, po::exclude_positional);
  print_warning_about_unrecognized_options(unrecognized);

  if (cat_file_vm.count("help")) {
    std::cout << cat_file_options << std::endl;
  } else if (!cat_file_vm.count("object")) {
    std::cout << "ERROR! Object name was not specified" << std::endl;
  } else {
    try {
      bool is_pretty_printing = cat_file_vm.count("pretty");
      actions::CatFileAction::cat_file(cat_file_vm["object"].as<std::string>(),
                                       is_pretty_printing);
    } catch (std::runtime_error e) {
      std::cout << "ERROR! " << e.what() << std::endl;
    }
  }
}

/// @brief This method is solely designed to be used in terminal subcommands.
/// Prints warning in which enumerates all unrecognized options.
/// @param unrecognized vector of unrecognized options.
void App::print_warning_about_unrecognized_options(
    std::vector<std::string>& unrecognized) {
  if (!unrecognized.empty()) {
    std::cout << "*Warning! These options were not recognized: ";
    for (std::size_t i = 0; i < unrecognized.size(); ++i) {
      std::cout << (i ? ", " : "") << '"' << unrecognized[i] << '"';
    }
    std::cout << std::endl;
  }
}
}  // namespace yit
