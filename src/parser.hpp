#pragma once


#include <optional>
#include <string>
#include <vector>


namespace parser {

    struct Command {
        std::string name;
        std::vector<std::string> args;
    };

    class ParseError : public std::exception {
        public:
        const char *what() const noexcept override {
            return "Failed to parse command";
        }
    };
}

std::optional<parser::Command> parse(std::string input);
