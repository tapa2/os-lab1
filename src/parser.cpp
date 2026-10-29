#include "parser.hpp"

namespace {

const char* const kWhitespace = " \t\r\n";

// Splits text into words separated by any amount of spaces/tabs.
std::vector<std::string> splitWords(const std::string& text) {
    std::vector<std::string> words;
    size_t start = text.find_first_not_of(kWhitespace);
    while (start != std::string::npos) {
        size_t end = text.find_first_of(kWhitespace, start);
        if (end == std::string::npos) {
            end = text.size();
        }
        words.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(kWhitespace, end);
    }
    return words;
}

// Splits text at every delimiter; "a&&b" gives {"a", "", "b"}.
std::vector<std::string> splitBy(const std::string& text, char delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    while (true) {
        size_t end = text.find(delimiter, start);
        if (end == std::string::npos) {
            parts.push_back(text.substr(start));
            return parts;
        }
        parts.push_back(text.substr(start, end - start));
        start = end + 1;
    }
}

// Parses one command (the text between '&'s). Returns false on a syntax error.
bool parseCommand(const std::string& text, Command& command) {
    std::vector<std::string> sides = splitBy(text, '>');
    if (sides.size() > 2) {
        return false;  // more than one '>'
    }

    command.args = splitWords(sides[0]);
    if (sides.size() == 2) {
        std::vector<std::string> files = splitWords(sides[1]);
        if (command.args.empty() || files.size() != 1) {
            return false;  // no command before '>', or not exactly one file after it
        }
        command.outputFile = files[0];
    }
    return true;
}

}  // namespace

bool parseLine(const std::string& line, std::vector<Command>& commands) {
    commands.clear();
    for (const std::string& text : splitBy(line, '&')) {
        Command command;
        if (!parseCommand(text, command)) {
            return false;
        }
        if (!command.args.empty()) {
            commands.push_back(command);
        }
    }
    return true;
}
