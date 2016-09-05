#ifndef HEADER_GUARD_SRCINFO_PARSER_H
#define HEADER_GUARD_SRCINFO_PARSER_H

#include <string>
#include <vector>

struct srcinfo {
    std::string name, description, version, release, url, license, architecture;
    std::vector<std::string> depends, optdepends, sources, backups;
};

srcinfo srcinfo_parser(const char* srcinfo_file);

#endif
