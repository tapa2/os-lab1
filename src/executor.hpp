#pragma once

#include <sys/types.h>

#include <string>
#include <vector>

#include "parser.hpp"

// Finds the program in searchPath and starts it in a child process.
// Returns the child's pid, or -1 if the program was not found or fork failed
// (the error message is already printed in that case).
pid_t launchCommand(const Command& command, const std::vector<std::string>& searchPath);

// Blocks until the given child process terminates.
void waitForChild(pid_t pid);
