#include "parser.hpp"

#include <cctype>

#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum State {
  InSingleQuote,
  InDoubleQuote,
  InDoubleQuoteAfterBackslash,
  AfterBackslash,
  AfterGt,
  Normal,
};

enum Action {
  WriteCmd,
  WriteTruncRedirect,
  WriteAppendRedirect,
  WriteArg,
};

bool is_number(const std::string &s) {
  for (char c : s) {
    if (!std::isdigit(c)) {
      return false;
    }
  }
  return true;
}

namespace parser {
std::optional<Command> parse(std::string input) {
  State state = Normal;
  std::string current_token{};
  Action action(WriteCmd);
  Action fallbackAction(WriteCmd);
  parser::Command command{};
  int redirect_src(1);
  auto flush_token = [&action, &fallbackAction, &command, &current_token, redirect_src]() {
    switch (action) {
    case WriteCmd:
      command.name = std::move(current_token);
      action = WriteArg;
      fallbackAction = WriteArg;
      break;
    case WriteTruncRedirect:
    case WriteAppendRedirect:
      command.redirects.push_back(
          Redirection{action == WriteTruncRedirect ? Redirection::Trunc
                                                   : Redirection::Append,
                      redirect_src, std::move(current_token)});
      action = fallbackAction;
      break;
    case WriteArg:
      command.args.push_back(std::move(current_token));
      break;
    }
  };
  bool enforce_write = false;
  auto flush_maybe = [&flush_token, &current_token, &enforce_write]() {
      if (enforce_write) {
          flush_token();
          enforce_write = false;
      } else if (!current_token.empty()) {
          flush_token();
      }
  };
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
    case AfterGt:
      state = Normal;
      if (c == '>') {
        action = WriteAppendRedirect;
        break;
      } else {
        action = WriteTruncRedirect;
      }
    case Normal:
      if (c == '\'') {
        state = InSingleQuote;
        enforce_write = true;
      } else if (c == '"') {
        state = InDoubleQuote;
        enforce_write = true;
      } else if (c == '\\') {
        state = AfterBackslash;
      } else if (c == '>') {
        std::cout << "current token: " << current_token << std::endl;
        try {
          redirect_src = std::stoi(current_token);
          current_token = "";
        } catch (std::invalid_argument const &exc) {
          redirect_src = 1;
          flush_maybe();
        }
        state = AfterGt;
      } else if (std::isspace(c)) {
          flush_maybe();
      } else {
        current_token += c;
      }
      break;
    }
  }

  flush_maybe();

  if (state == InSingleQuote) {
    throw parser::ParseError();
  } else if (state == InDoubleQuote) {
    throw parser::ParseError();
  } else if (action == WriteTruncRedirect || action == WriteAppendRedirect) {
    throw parser::ParseError();
  }

  if (action == WriteCmd) {
      return std::nullopt;
  }

  return command;
}
} // namespace parser
