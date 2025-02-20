#include "parser.hpp"

#include <cctype>
#include <optional>
#include <string>
#include <vector>

enum State {
  InSingleQuote,
  InDoubleQuote,
  InDoubleQuoteAfterBackslash,
  AfterBackslash,
  Normal,
};

std::optional<parser::Command> parse(std::string input) {
  std::optional<parser::Command> command;
  auto push_token = [&command](std::string token) {
    if (command.has_value()) {
      command->args.push_back(token);
    } else {
      command = parser::Command({token});
    }
  };
  std::string current_token{};
  State state = Normal;
  for (auto c : input) {
    switch (state) {
    case InSingleQuote:
      if (c == '\'') {
        state = Normal;
      } else {
        current_token += c;
      }
      break;
    case InDoubleQuote:
      if (c == '"') {
        state = Normal;
      } else if (c == '\\') {
        state = InDoubleQuoteAfterBackslash;
      } else {
        current_token += c;
      }
      break;
    case InDoubleQuoteAfterBackslash:
      switch (c) {
      case '\\':
      case '"':
      case '$':
        current_token += c;
        break;
      default:
        current_token += '\\';
        current_token += c;
        break;
      }
      state = InDoubleQuote;
      break;
    case AfterBackslash:
      current_token += c;
      state = Normal;
      break;
    case Normal:
      if (c == '\'') {
        state = InSingleQuote;
      } else if (c == '"') {
        state = InDoubleQuote;
      } else if (c == '\\') {
        state = AfterBackslash;
      } else if (std::isspace(c)) {
        if (!current_token.empty()) {
          push_token(current_token);
          current_token.clear();
        }
      } else {
        current_token += c;
      }
      break;
    }
  }
  if (state == InSingleQuote) {
    throw parser::ParseError();
  } else if (state == InDoubleQuote) {
    throw parser::ParseError();
  }
  if (!current_token.empty()) {
    push_token(current_token);
  }
  return command;
}
