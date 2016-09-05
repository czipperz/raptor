#include "initialize_package.hh"
#include "archive.hh"
#include "curl.hh"
#include "file.hh"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>

static void init_error(CURLcode res) {
    if (res != CURLE_OK) {
        std::cerr << "ERROR problem initializing curl: "
                  << curl_easy_strerror(res) << std::endl;
        throw EXIT_FAILURE;
    }
}

static void extract(const char* filename) {
    LibArchive::ReadArchive archive;
    LibArchive::WriteDisk extracted;
    LibArchive::Entry entry;
    int status;

    archive_read_support_format_all(archive);
    // archive_read_support_compression_all(archive);

    archive_write_disk_set_options(extracted,
                                   ARCHIVE_EXTRACT_TIME |
                                       ARCHIVE_EXTRACT_FFLAGS |
                                       ARCHIVE_EXTRACT_ACL |
                                       ARCHIVE_EXTRACT_PERM);
    archive_write_disk_set_standard_lookup(extracted);

    if ((status = archive_read_open_filename(archive, filename, 10240))) {
        std::fprintf(stderr, "ERROR opening archive %s: %s\n",
                     filename, archive_error_string(archive));
        throw EXIT_FAILURE;
    }
    while ((status = archive_read_next_header2(archive, entry)) !=
           ARCHIVE_EOF) {
        if (status < ARCHIVE_OK) {
            std::fprintf(stderr, "ERROR extracting %s: %s\n",
                         filename, archive_error_string(archive));
        }
        if (status < ARCHIVE_WARN) {
            throw EXIT_FAILURE;
        }
        status = archive_write_header(extracted, entry);
        if (status < ARCHIVE_OK) {
            std::cerr << "";
        }
    }
}

struct str {
    str(size_t num)
        : raw(new char[num]) {}
    str(const str& other)
        : raw(new char[std::strlen(other.raw)]) {
        std::strcpy(raw, other);
    }
    str(const char* cstr)
        : raw(new char[std::strlen(cstr)]) {
        std::strcpy(raw, cstr);
    }
    str(char* cstr, bool copy = true) {
        if (copy) {
            raw = new char[std::strlen(cstr)];
            std::strcpy(raw, cstr);
        } else {
            raw = cstr;
        }
    }
    ~str() { delete[] raw; }
    str& operator=(const str& other) {
        str copy(other);
        delete[] raw;
        raw = copy;
        copy.raw = NULL;
        return *this;
    }
    operator char*() { return raw; }
    operator const char*() const { return raw; }

private:
    str() {}
    char* raw;
};

static void
pkgname2(std::string& package, bool& download, bool& extract) {
    download = true;
    extract = true;
}

static void
pkgname(std::string& package, bool& download, bool& extract) {
    struct stat st;
    if (stat(package.c_str(), &st) == 0) {
        if ((st.st_mode & S_IFMT) == S_IFREG) {
        } else if ((st.st_mode & S_IFMT) == S_IFDIR) {
            download = false;
            extract = false;
        } else {
            std::cerr << "Unsupported file type of '" << package
                      << "' (";
            switch (st.st_mode & S_IFMT) {
                case S_IFSOCK:
                    std::cerr << "socket";
                    break;
                case S_IFBLK:
                    std::cerr << "block device";
                    break;
                case S_IFCHR:
                    std::cerr << "character device";
                    break;
                case S_IFIFO:
                    std::cerr << "FIFO";
                    break;
                default:
                    std::cerr << "other - " << (st.st_mode & S_IFMT);
                    break;
            }
            std::cerr << ")" << std::endl;
            throw EXIT_FAILURE;
        }
    } else if (std::strchr(package.c_str(), '/')) {
        std::cerr << "File not found (packages shouldn't have '/'s): "
                  << package << std::endl;
        throw EXIT_FAILURE;
    }
}

void initialize_package(const char* package) {
    struct stat st;
    std::string package_name = package;
    std::string folder_name = "/tmp/";

    // note that when download is true, so is extract
    bool download = true, extract = true;
    pkgname(package_name, download, extract);

    folder_name += '_';
    std::time_t t = std::time(NULL);
    std::tm tm = *std::localtime(&t);

    // /tmp/PACKAGE_MON-DAY-YEAR_NUM
    // NUM is to prevent overrides of previous builds
    char buffer[100];
    std::sprintf(buffer, "%d", tm.tm_mon + 1);
    folder_name += buffer;
    folder_name += '-';
    std::sprintf(buffer, "%d", tm.tm_mday);
    folder_name += buffer;
    folder_name += '-';
    std::sprintf(buffer, "%d", tm.tm_year + 1900);
    folder_name += buffer;
    folder_name += '_';

    int number;
    for (number = 1; ; ++number) {
        std::sprintf(buffer, "%d", number);
        // if this temporary directory doesn't exist, use it
        if (stat((folder_name + buffer).c_str(), &st) == -1) {
            folder_name += buffer;
            break;
        }
    }
    std::cout << folder_name << std::endl;

    mkdir(folder_name.c_str(), 0700);
    chdir(folder_name.c_str());

    LibCurl::Easy curl;
    LibCurl::Code res;

    std::string file_name;
    file_name += package;
    file_name += ".tar.gz";
    LibFile::OFile file(file_name);

    std::string url =
        "https://aur.archlinux.org/cgit/aur.git/snapshot/" +
        file_name;

    // handle errors in setup
    init_error(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
    init_error(curl_easy_setopt(curl, CURLOPT_WRITEDATA, file.raw));
    init_error(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L));

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "ERROR problem downloading page " << url << ": "
                  << curl_easy_strerror(res) << std::endl;
        throw EXIT_FAILURE;
    }

    extract();
}
