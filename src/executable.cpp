#include "executable.hpp"

// for now we only support linux using fork and exec
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

namespace cmd {
    Executable::Executable(fs::path entry) : path(entry) {}
    std::string Executable::get_name() { return path.filename(); }
    std::string Executable::get_desc() { return path; }
    int Executable::execute(std::vector<std::string> args, const Ctx &ctx) {
      std::vector<const char*> c_args;
      c_args.push_back(get_name().c_str());
      for (const auto& arg : args) {
          c_args.push_back(arg.c_str());
      }
      c_args.push_back(nullptr);
      execvp(path.c_str(), const_cast<char* const*>(c_args.data()));
      std::cerr << get_name() << ": Failed to exec" << std::endl;
      _exit(1);
    }
} // namespace cmd
