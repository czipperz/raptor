#include "package_map_parser.hh"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <locale>

static int parse_line(const std::string& line,
                      std::map<std::string, std::string>& mappings) {
    // skip empty lines and those starting in #
    if (line.empty()) {
    } else if (line[0] == '#') {
    } else {
        std::string::const_iterator itr = line.begin(), beg;
        // `|  asdf  asdf` -> `  |asdf  asdf`
        while (std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                // skip lines with only spaces
                return 0;
            }
        }
        beg = itr;
        // `  |asdf  asdf` -> `  asdf|  asdf`
        while (not std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                // only one word error
                return 1;
            }
        }
        std::string aur(beg, itr);
        // `  asdf|  asdf` -> `  asdf  |asdf`
        while (std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                return 1;
            }
        }
        beg = itr;
        // `  asdf  |asdf` -> `  asdf  asdf|`
        while (not std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                break;
            }
        }
        std::string apt(beg, itr);
        if (itr != line.end()) {
            // skip trailing spaces
            while (std::isspace(*itr)) {
                ++itr;
                if (itr == line.end()) {
                    break;
                }
            }
            // trailing word
            if (itr != line.end() and *itr != '#') {
                return 2;
            }
        }
        mappings.insert(std::pair<std::string, std::string>(
                            aur, apt));
    }
    return 0;
}

std::map<std::string, std::string>
package_map_parser(const char* file_name) {
    std::ifstream file(file_name);
    if (not file) {
        std::cerr << "ERROR can't open file: " << file_name
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::map<std::string, std::string> mappings;
    std::string line;
    std::size_t linenum = 0;
    while (std::getline(file, line)) {
        ++linenum;
        switch (parse_line(line, mappings)) {
            case 1:
                std::cerr << "Parse error on package map file - only "
                             "one word on this line (should be "
                             "two)\n";
                goto linerr;
            case 2:
                std::cerr << "Parse error on package map file - more "
                             "than two words on this line (should be "
                             "two)\n";
            linerr:
                std::cerr << file_name << ":" << linenum << ":"
                          << line << std::endl;
                std::exit(EXIT_FAILURE);
                break;
        }
    }
    return mappings;
}
