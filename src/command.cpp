#include "command.hpp"

#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

namespace cmd {
Command::Command() {}
void Command::redirect(int src, int dest) {
  redirects.push_back({src, dest});
}
int Command::exec(std::vector<std::string> args, const Ctx &ctx) {
  auto pid = fork();
    if (pid == -1) {
        std::perror("fork");
        return 1;
    } else if (pid == 0) {
        for (auto [src, target] : redirects) {
            if (dup2(target, src) == -1) {
                std::perror("dup2");
                return 1;
            }
        }
        exit(execute(args, ctx));
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            std::cerr << get_name() << ": Child process did not exit normally" << std::endl;
            return 1;
        }
    }
}
}; // namespace cmd
