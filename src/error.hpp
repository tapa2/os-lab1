#pragma once

#include <unistd.h>

// The one and only error message required by the specification.
// write() is unbuffered, so the message reaches stderr immediately and
// cannot get reordered with (or duplicated by) buffered stdio output.
inline void printError() {
    static const char kErrorMessage[] = "An error has occurred\n";
    ssize_t written = write(STDERR_FILENO, kErrorMessage, sizeof(kErrorMessage) - 1);
    (void)written;  // nothing sensible to do if writing to stderr fails
}
