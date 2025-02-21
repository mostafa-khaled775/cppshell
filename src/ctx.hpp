#pragma once

class Ctx;

#include "command.hpp"

#include <cstdlib>
#include <memory>
#include <optional>
#include <string>

class Ctx {
public:
  Ctx();
  std::optional<std::unique_ptr<cmd::Command>>
  find_command(std::string name) const;
};
