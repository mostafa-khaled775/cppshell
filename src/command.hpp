#pragma once

namespace cmd {
class Command;
};

#include "ctx.hpp"

#include <string>
#include <vector>

namespace cmd {
class Command {
public:
  virtual int execute(std::vector<std::string> args, const Ctx &ctx) = 0;
  virtual std::string get_name() = 0;
  virtual std::string get_desc() = 0;
  virtual ~Command() = default;
};
}; // namespace cmd
