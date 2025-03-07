#include "ctx.hpp"
#include "builtins.hpp"
#include "command.hpp"
#include "executable.hpp"

#include <filesystem>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

Ctx::Ctx() {}

std::optional<std::unique_ptr<cmd::Command>>
Ctx::find_command(std::string name) const {
  if (name.empty()) {
    return std::nullopt;
  } else if (name.contains('/')) {
    return std::make_unique<cmd::Executable>(name);
  } else if (auto builtin = cmd::get_builtin(name); builtin.has_value()) {
    return std::move(builtin);
  } else {
    char *path = std::getenv("PATH");
    if (path != nullptr) {
      std::string dir;
      std::istringstream path_iss(path);
      while (std::getline(path_iss, dir, ':')) {
        if (fs::is_directory(dir)) {
          if (auto p = fs::path(dir) / name; fs::exists(p)) {
            return std::make_unique<cmd::Executable>(p);
          }
        }
      }
    }
    return std::nullopt;
  }
}
