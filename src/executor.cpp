#include "executor.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>

#include "error.hpp"

namespace {

// Returns "<dir>/<name>" for the first directory in searchPath that contains
// an executable file with that name, or an empty string if there is none.
std::string findExecutable(const std::string& name, const std::vector<std::string>& searchPath) {
    for (const std::string& dir : searchPath) {
        std::string candidate = dir + "/" + name;
        if (access(candidate.c_str(), X_OK) == 0) {
            return candidate;
        }
    }
    return "";
}

}  // namespace

pid_t launchCommand(const Command& command, const std::vector<std::string>& searchPath) {
    std::string program = findExecutable(command.args[0], searchPath);
    if (program.empty()) {
        printError();
        return -1;
    }

    // execv() expects a NULL-terminated array of C strings. It never modifies
    // them; the parameter is non-const only for historical C reasons.
    std::vector<char*> argv;
    for (const std::string& arg : command.args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    // fork() copies the stdio buffer too: flush it so pending output
    // is not printed twice (once by each process).
    std::fflush(stdout);

    pid_t pid = fork();
    if (pid == -1) {
        printError();
        return -1;
    }
    if (pid == 0) {
        // Child: replace this process image with the program.
        execv(program.c_str(), argv.data());
        // execv() returns only on failure.
        printError();
        _exit(1);
    }
    return pid;  // parent
}

void waitForChild(pid_t pid) {
    if (waitpid(pid, nullptr, 0) == -1) {
        printError();
    }
}
