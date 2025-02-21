#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "command.hpp"
#include "ctx.hpp"
#include "parser.hpp"
#include "shell.hpp"


Shell::Shell() : ctx() {}

int Shell::repl() {
  std::cout << "$ ";
  if (std::cin.eof()) {
    return 0;
  }
  std::string input;
  std::getline(std::cin, input);
  if (auto parsed_cmd = parse(input); parsed_cmd.has_value()) {
    auto [command, args] = parsed_cmd.value();
    // currently exit command has to be treated separately
    if (command == "exit") {
      return 0;
    }

    if (auto cmd = ctx.find_command(command); cmd.has_value()) {
      cmd.value()->execute(args, ctx);
    } else {
      std::cout << command << ": command not found" << std::endl;
      return repl();
    }

  } else {
    std::cout << std::endl;
  }

  return repl();
}
