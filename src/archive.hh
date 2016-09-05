/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2016 Chris Gregory czipperz@gmail.com
 */

#ifndef HEADER_GUARD_ARCHIVE_H
#define HEADER_GUARD_ARCHIVE_H

#include <archive.h>
#include <archive_entry.h>

#if __cplusplus < 201103L
#define LibArchive_NOEXCEPT
#else
#define LibArchive_NOEXCEPT noexcept
#endif

namespace LibArchive {
/** Read from an archive */
struct ReadArchive {
    ReadArchive() LibArchive_NOEXCEPT : raw(archive_read_new()) {}
    ~ReadArchive() LibArchive_NOEXCEPT {
        archive_read_close(raw);
        archive_read_free(raw);
    }
    archive* raw;
    operator archive*() LibArchive_NOEXCEPT { return raw; }

private:
    ReadArchive(const ReadArchive&);
    ReadArchive& operator=(const ReadArchive&);
};

/** Superclass of WriteArchive and WriteDisk */
struct Write {
    ~Write() LibArchive_NOEXCEPT {
        archive_write_close(raw);
        archive_write_free(raw);
    }
    archive* raw;
    operator archive*() LibArchive_NOEXCEPT { return raw; }

protected:
    Write(archive* raw) LibArchive_NOEXCEPT : raw(raw) {}

private:
    Write(const Write&);
    Write& operator=(const Write&);
};

/** Write to an archive */
struct WriteArchive : public Write {
    WriteArchive() LibArchive_NOEXCEPT : Write(archive_write_new()) {}

private:
    WriteArchive(const WriteArchive&);
    WriteArchive& operator=(const WriteArchive&);
};

/** Write objects read from an archive to disk */
struct WriteDisk : public Write {
    WriteDisk() LibArchive_NOEXCEPT : Write(archive_write_disk_new()) {}

private:
    WriteDisk(const WriteDisk&);
    WriteDisk& operator=(const WriteDisk&);
};

/** Entry in archive */
struct Entry {
    Entry() LibArchive_NOEXCEPT : raw(archive_entry_new()) {}
    ~Entry() LibArchive_NOEXCEPT { archive_entry_free(raw); }
    archive_entry* raw;
    operator archive_entry*() LibArchive_NOEXCEPT { return raw; }

private:
    Entry(const Entry&);
    Entry& operator=(const Entry&);
};
}

#endif
