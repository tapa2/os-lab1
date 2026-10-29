#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "error.hpp"
#include "parser.hpp"

namespace {

// Handles one input line (without the trailing newline).
// TODO: replace the debug print with execution in the next stage.
void processLine(const std::string& line) {
    std::vector<Command> commands;
    if (!parseLine(line, commands)) {
        printError();
        return;
    }

    for (size_t i = 0; i < commands.size(); ++i) {
        std::printf("[debug] command %zu:", i + 1);
        for (const std::string& arg : commands[i].args) {
            std::printf(" [%s]", arg.c_str());
        }
        if (!commands[i].outputFile.empty()) {
            std::printf(" > [%s]", commands[i].outputFile.c_str());
        }
        std::printf("\n");
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
        processLine(line);
    }

    std::free(buffer);
    if (input != stdin) {
        std::fclose(input);
    }
    std::exit(0);
}
