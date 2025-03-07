#pragma once


#include <optional>
#include <string>
#include <vector>


namespace parser {

    struct Redirection {
        enum Type {
            Trunc,
            Append,
        };
        Type type;
        int src;
        std::string dest;
    };
    struct Command {
        std::string name;
        std::vector<std::string> args;
        std::vector<Redirection> redirects;
    };

    class ParseError : public std::exception {
        public:
        const char *what() const noexcept override {
            return "Failed to parse command";
        }
    };
    std::optional<Command> parse(std::string input);
}
