#include <cstdio>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

#include "command.hpp"
#include "ctx.hpp"
#include "parser.hpp"
#include "shell.hpp"


Shell::Shell() : ctx() {}

int Shell::repl() {
  if (std::cin.eof()) {
    return 0;
  }
  std::cout << "$ ";
  std::string input;
  std::getline(std::cin, input);
  try {
    auto [command, args, redirects] = parser::parse(input).value();
    if (command == "exit") {
      return 0;
    }

    if (auto cmd = ctx.find_command(command); cmd.has_value()) {
      for (const auto &[type, src, dest] : redirects) {
          auto mode = type == parser::Redirection::Trunc ? O_TRUNC : O_APPEND;
          cmd.value()->redirect(src,open(dest.c_str(), mode | O_CREAT | O_WRONLY, 0644));
      }
      cmd.value()->exec(args, ctx);
    } else {
      std::cout << command << ": command not found" << std::endl;
      return repl();
    }
  }
  catch (std::exception &e) {

  }



  return repl();
}
