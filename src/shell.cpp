#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "builtins.hpp"
#include "command.hpp"
#include "ctx.hpp"
#include "executable.hpp"
#include "shell.hpp"

namespace fs = std::filesystem;

auto executables(std::string path) {
  std::string dir;
  std::stringstream path_ss(path);

  std::vector<std::unique_ptr<cmd::Command>> commands;
  while (std::getline(path_ss, dir, ':')) {
    if (!fs::exists(dir)) {
        std::cerr << "Path " << dir << " does not exist" << std::endl;
        continue;
    }
    if (!fs::is_directory(dir)) {
        std::cerr << "Path " << dir << " is not a directory" << std::endl;
        continue;
    }
    for (auto const &entry : fs::directory_iterator(dir)) {
      if (entry.is_regular_file() &&
          (fs::status(entry).permissions() & fs::perms::owner_exec) !=
              fs::perms::none) {
        commands.push_back(std::move(std::make_unique<cmd::Executable>(entry)));
      }
    }
  }
  return commands;
}

Shell::Shell() {
  auto path_commands = executables(std::getenv("PATH"));
  for (auto it = path_commands.rbegin(); it != path_commands.rend(); ++it) {
    ctx.commands[(*it)->get_name()] = std::move(*it);
  }
  for (auto &command : cmd::builtins()) {
    ctx.commands[command->get_name()] = std::move(command);
  }
}

int Shell::repl() {
  std::cout << "$ ";
  if (std::cin.eof()) {
    return 0;
  }
  std::string input;
  std::getline(std::cin, input);
  std::stringstream ss(input);
  std::string command;
  ss >> command;

  // currently exit command has to be treated separately
  if (command == "exit") {
    return 0;
  }

  const auto it = ctx.commands.find(command);
  if (it == ctx.commands.end()) {
    std::cout << command << ": command not found" << std::endl;
    return repl();
  }
  const auto &cmd = it->second;
  std::vector<std::string> args;
  std::string arg;
  while (ss >> arg) {
    args.push_back(arg);
  }
  cmd->execute(args, ctx);
  return repl();
}
