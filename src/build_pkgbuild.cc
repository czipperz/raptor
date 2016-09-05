#include "build_pkgbuild.hh"
#include <cstdlib>
#include <cstdio>
#include <iostream>

void build_pkgbuild() {
    std::fflush(stdout);
    std::fflush(stderr);
    int errcode = std::system("/usr/bin/env bash -c '\
source PKGBUILD || exit 1;\
which pkgver >/dev/null 2>&1 && pkgver;\
which prepare >/dev/null 2>&1 && prepare;\
which build >/dev/null 2>&1 && build;\
which check >/dev/null 2>&1 && check;\
which package >/dev/null 2>&1 && package;\
'");
    std::fflush(stdout);
    std::fflush(stderr);
    if (errcode != EXIT_SUCCESS) {
        std::cerr << "ERROR building package failed." << std::endl;
        std::exit(errcode);
    }
}
