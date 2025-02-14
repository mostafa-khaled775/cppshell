#pragma once

#include <vector>
#include <string>

namespace cmd {
    class Command;
};

#include "ctx.hpp"


namespace cmd {
    class Command {
        public:
        virtual int execute(std::vector<std::string> args, const Ctx& ctx) = 0;
        virtual std::string get_name() = 0;
        virtual std::string get_desc() = 0;
        virtual ~Command() = default;
    };
};
