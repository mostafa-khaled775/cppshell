#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class Ctx;

#include "command.hpp"

class Ctx {
    public:
    std::unordered_map<std::string, std::unique_ptr<cmd::Command>> commands;
};
