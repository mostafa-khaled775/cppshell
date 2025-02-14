#pragma once

#include <filesystem>

namespace fs = std::filesystem;

#include "command.hpp"

namespace cmd {
    class Executable : public cmd::Command {
        private:
         fs::path path;

        public:
        Executable(fs::directory_entry entry);
        std::string get_name() override;
        std::string get_desc() override;
        int execute(std::vector<std::string> args, const Ctx &ctx) override;
    };
}
