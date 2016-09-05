#ifndef HEADER_GUARD_FILE_H
#define HEADER_GUARD_FILE_H

#include <cstdio>
#include <cstring>
#include <string>

#if __cplusplus < 201103L
#define LibFile_NOEXCEPT
#else
#define LibFile_NOEXCEPT noexcept
#endif

namespace LibFile {
struct File {
    ~File() LibFile_NOEXCEPT {
        if (raw) {
            std::fclose(raw);
        }
    }
    operator std::FILE*() const LibFile_NOEXCEPT { return raw; }
    std::FILE* raw;

    int flush() LibFile_NOEXCEPT { return std::fflush(*this); }

    void clearerr() LibFile_NOEXCEPT { return std::clearerr(*this); }

protected:
    File(std::FILE* raw) LibFile_NOEXCEPT : raw(raw) {}

private:
    File(const File&);
    File& operator=(const File&);
};

struct IFile : public File {
    IFile(const char* fname) LibFile_NOEXCEPT
        : File(std::fopen(fname, "r")) {}
    IFile(const std::string& fname) LibFile_NOEXCEPT
        : File(std::fopen(fname.c_str(), "r")) {}

    int get_c() { return std::fgetc(*this); }
    char* get_s(char* str, int num) {
        return std::fgets(str, num, *this);
    }
    std::string get(int num) {
        std::string res;
        res.resize(num);
        get_s(const_cast<char*>(res.c_str()), num);
        return res;
    }

    int eof() const { return std::feof(*this); }

private:
    IFile(const IFile&);
    IFile& operator=(const IFile&);
};

struct OFile : public File {
    OFile(const char* fname) LibFile_NOEXCEPT
        : File(std::fopen(fname, "w")) {}
    OFile(const std::string& fname) LibFile_NOEXCEPT
        : File(std::fopen(fname.c_str(), "w")) {}

    // this macro prevents putc from expanding as a macro (which it
    // might be)
    int put_c(int c) { return std::fputc(c, *this); }
    int put_s(const char* str) { return std::fputs(str, *this); }

    int put(int c) { return put_c(c); }
    int put(const char* str) { return put_s(str); }

private:
    OFile(const OFile&);
    OFile& operator=(const OFile&);
};
}

#endif
