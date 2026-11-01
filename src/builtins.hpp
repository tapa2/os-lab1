#pragma once

#include <string>
#include <vector>

#include "parser.hpp"

// Runs the command inside the shell process if it is a built-in
// (exit, cd, path) and returns true. Returns false for any other command,
// which must then be started as an external program.
bool runBuiltin(const Command& command, std::vector<std::string>& searchPath);
