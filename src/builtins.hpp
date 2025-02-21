#pragma once

#include <optional>
#include <memory>

#include "command.hpp"

namespace cmd {
    std::optional<std::unique_ptr<cmd::Command>> get_builtin(std::string name);
};
