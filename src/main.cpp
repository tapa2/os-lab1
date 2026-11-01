#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "builtins.hpp"
#include "error.hpp"
#include "executor.hpp"
#include "parser.hpp"

namespace {

// Handles one input line (without the trailing newline).
// For now commands run one after another; '&' parallelism comes later.
void processLine(const std::string& line, std::vector<std::string>& searchPath) {
    std::vector<Command> commands;
    if (!parseLine(line, commands)) {
        printError();
        return;
    }

    for (const Command& command : commands) {
        if (runBuiltin(command, searchPath)) {
            continue;
        }
        pid_t pid = launchCommand(command, searchPath);
        if (pid > 0) {
            waitForChild(pid);
        }
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc > 2) {
        printError();
        std::exit(1);
    }

    FILE* input = stdin;
    const bool interactive = (argc == 1);
    if (!interactive) {
        input = std::fopen(argv[1], "r");
        if (input == nullptr) {
            printError();
            std::exit(1);
        }
    }

    // Directories searched for programs; changed later by the `path` built-in.
    std::vector<std::string> searchPath{"/bin"};

    // getline() allocates and grows the buffer itself, so lines of any length work.
    char* buffer = nullptr;
    size_t capacity = 0;

    while (true) {
        if (interactive) {
            std::printf("wish> ");
            std::fflush(stdout);  // the prompt has no '\n', so flush it explicitly
        }

        ssize_t length = getline(&buffer, &capacity, input);
        if (length == -1) {
            break;  // EOF
        }

        std::string line(buffer, static_cast<size_t>(length));
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }
        processLine(line, searchPath);
    }

    std::free(buffer);
    if (input != stdin) {
        std::fclose(input);
    }
    std::exit(0);
}
