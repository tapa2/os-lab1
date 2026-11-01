#include "builtins.hpp"

#include <unistd.h>

#include <cstdlib>

#include "error.hpp"

namespace {

// exit: takes no arguments.
void builtinExit(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        printError();
        return;
    }
    std::exit(0);
}

// cd <dir>: exactly one argument. Must run in the shell itself: a child
// process would change only its own working directory.
void builtinCd(const std::vector<std::string>& args) {
    if (args.size() != 2 || chdir(args[1].c_str()) == -1) {
        printError();
    }
}

// path [dir ...]: replaces the whole search path (may become empty).
void builtinPath(const std::vector<std::string>& args, std::vector<std::string>& searchPath) {
    searchPath.assign(args.begin() + 1, args.end());
}

}  // namespace

bool runBuiltin(const Command& command, std::vector<std::string>& searchPath) {
    const std::string& name = command.args[0];
    if (name == "exit") {
        builtinExit(command.args);
    } else if (name == "cd") {
        builtinCd(command.args);
    } else if (name == "path") {
        builtinPath(command.args, searchPath);
    } else {
        return false;
    }
    return true;
}
