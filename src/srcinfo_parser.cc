#include "srcinfo_parser.hh"
#include <fstream>
#include <iostream>
#include <cstdlib>

static int parse_line(const std::string& line,
                      srcinfo& res) {
    std::string::const_iterator itr = line.begin(), beg;
    if (line.size()) {
        while (std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                return 0;
            }
        }
        if (*itr == '#') {
            return 0;
        }
        beg = itr;
        while (not std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                return 0;
            }
        }
        std::string key(beg, itr);
        while (std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                return 1;
            }
        }
        if (*itr != '=') {
            return 2;
        }
        ++itr;
        while (std::isspace(*itr)) {
            ++itr;
            if (itr == line.end()) {
                return 1;
            }
        }
        std::string value(itr, line.end());
        if (key == "pkgbase" or key == "pkgname") {
            res.name = value;
        } else if (key == "pkgdesc") {
            res.description = value;
        } else if (key == "pkgver") {
            res.version = value;
        } else if (key == "pkgrel") {
            res.release = value;
        } else if (key == "url") {
            res.url = value;
        } else if (key == "arch") {
            res.architecture = value;
        } else if (key == "license") {
            res.license = value;
        } else if (key == "depends") {
            res.depends.push_back(value);
        } else if (key == "optdepends") {
            res.optdepends.push_back(value);
        } else if (key == "source") {
            res.sources.push_back(value);
        } else if (key == "sha256sums") {
            // ignored
        } else if (key == "backup") {
            res.backups.push_back(value);
        } else {
            std::cerr << "Unrecognized key '" << key << "'\n";
            return 3;
        }
    }
    return 0;
}

srcinfo srcinfo_parser(const char* srcinfo_file) {
    std::ifstream file(srcinfo_file);
    if (not file) {
        std::cerr << "ERROR can't open file: " << srcinfo_file
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
    srcinfo res;
    std::string line;
    std::size_t linenum = 0;
    while (std::getline(file, line)) {
        ++linenum;
        switch (parse_line(line, res)) {
            case 1:
                std::cerr << "Parse error on srcinfo file - only one "
                             "word on this line (should be `key = "
                             "value`)\n";
                goto linerr;
            case 2:
                std::cerr << "Equals sign not found after key\n";
                goto linerr;
            case 3:
                goto linerr;
            linerr:
                std::cerr << srcinfo_file << ":" << linenum << ":"
                          << line << std::endl;
                std::exit(EXIT_FAILURE);
        }
    }
    return res;
}
