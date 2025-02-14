#pragma once

#include "ctx.hpp"

class Shell {
    private:
    Ctx ctx;
    public:
    Shell();
    int repl();
};
