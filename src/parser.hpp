#pragma once

#include <string>
#include <vector>

// One command of an input line, e.g. "ls -la /tmp > out".
struct Command {
    std::vector<std::string> args;  // args[0] is the program name
    std::string outputFile;         // empty if the output is not redirected
};

// Splits a line into '&'-separated commands. Empty commands are skipped,
// so an empty line yields an empty vector. Returns false on a syntax error
// (misplaced or repeated '>'); printing the error is up to the caller.
bool parseLine(const std::string& line, std::vector<Command>& commands);
