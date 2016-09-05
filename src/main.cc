#include <curl/curl.h>
#include <cstdlib>
#include <iostream>

#include "fix_depends_versions.hh"
#include "fix_optdepends.hh"
#include "initialize_package.hh"
#include "package_map_parser.hh"
#include "show_list.hh"
#include "srcinfo_parser.hh"

int main(int argc, char** argv) {
    try {
        curl_global_init(CURL_GLOBAL_ALL);
        std::atexit(&curl_global_cleanup);
        --argc;
        ++argv;
        if (argc) {
            initialize_package(argv[0]);
            // srcinfo info = srcinfo_parser(argv[0]);
            // fix_depends_versions(info);
            // fix_optdepends(info);
            // std::cout << "name: " << info.name << std::endl
            //           << "description: " << info.description <<
            //           std::endl
            //           << "version: " << info.version << std::endl
            //           << "release: " << info.release << std::endl
            //           << "url: " << info.url << std::endl
            //           << "license: " << info.license << std::endl
            //           << "arch: " << info.architecture <<
            //           std::endl;
            // std::cout << "depends: ";
            // show_list(info.depends);
            // std::cout << "optdepends: ";
            // show_list(info.optdepends);
            // std::cout << "sources: ";
            // show_list(info.sources);
            // std::cout << "backups: ";
            // show_list(info.backups);
        }
    } catch (int ecode) {
        std::exit(ecode);
    }
}
