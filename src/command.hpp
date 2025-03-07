#pragma once

namespace cmd {
class Command;
};

#include "ctx.hpp"

#include <string>
#include <vector>

namespace cmd {
struct Redirection {
  int src;
  int dest;
};

class Command {
private:
  std::vector<Redirection> redirects;
public:
  void redirect(int src, int dest);
  virtual int exec(std::vector<std::string> args, const Ctx &ctx) final ;
  virtual int execute(std::vector<std::string> args, const Ctx &ctx) = 0;
  virtual std::string get_name() = 0;
  virtual std::string get_desc() = 0;
  Command();
  virtual ~Command() = default;
};
}; // namespace cmd
