#include "command.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

namespace cmd {
    class Builtin : public Command {
        public:
        std::string get_desc() override {
            return "a shell builtin";
        }
    };
    class Echo : public Builtin {
        public:
        std::string get_name() override {
            return "echo";
        }
        int execute(std::vector<std::string> args, const Ctx& ctx) override {
            for (const auto& arg : args) {
                std::cout << arg << " ";
            }
            std::cout << std::endl;
            return 0;
        }
    };

    class Type : public Builtin {
        std::string get_name() override {
            return "type";
        }
        int execute(std::vector<std::string> args, const Ctx& ctx) override {
            if (args.empty()) {
                std::cerr << "type: missing argument" << std::endl;
                return 1;
            }
            const auto& command_name = args[0];
            const auto it = ctx.commands.find(command_name);
            if (it == ctx.commands.end()) {
                std::cerr << command_name << ": not found" << std::endl;
                return 1;
            }
            const auto& command = it->second;
            std::cout << command_name << " is " << command->get_desc() << std::endl;
            return 0;
        }
    };

    class Exit : public Builtin {
        std::string get_name() override {
            return "exit";
        }
        int execute(std::vector<std::string> args, const Ctx& ctx) override {
            return 0;
        }
    };

    class Pwd: public Builtin {
        std::string get_name() override {
            return "pwd";
        }
        int execute(std::vector<std::string> args, const Ctx& ctx) override {
            std::cout << fs::current_path().string() << std::endl;
            return 0;
        }
    };

    class Cd: public Builtin {
        std::string get_name() override {
            return "cd";
        }
        int execute(std::vector<std::string> args, const Ctx& ctx) override {
            if (args.empty()) {
                std::cerr << "cd: missing argument" << std::endl;
                return 1;
            }
            const auto& path = args[0];
            std::error_code ec;
            if (auto pos = path.find('/'); path.substr(0, pos) == "~") {
                fs::current_path(fs::path(std::getenv("HOME")), ec);
            } else {
                fs::current_path(path, ec);
            }
            if (ec) {
                std::cerr << "cd: " << path << ": " << ec.message() << std::endl;
                return 1;
            }
            return 0;
        }
    };

    std::vector<std::unique_ptr<Command>> builtins() {
        std::vector<std::unique_ptr<Command>> builtins{};
        builtins.push_back(std::make_unique<Echo>());
        builtins.push_back(std::make_unique<Type>());
        builtins.push_back(std::make_unique<Exit>());
        builtins.push_back(std::make_unique<Pwd>());
        builtins.push_back(std::make_unique<Cd>());
        return builtins;
    }
};
