#pragma once

#include <vector>
#include <memory>

#include "command.hpp"

namespace cmd {
    std::vector<std::unique_ptr<cmd::Command>> builtins();
};
