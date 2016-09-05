#ifndef HEADER_GUARD_FIX_DEPENDS_VERSIONS_H
#define HEADER_GUARD_FIX_DEPENDS_VERSIONS_H

#include "srcinfo_parser.hh"

/// Take depends that look like
///     libc6>=2.15
/// and turn them into
///     libc6 (>= 2.15)
void fix_depends_versions(srcinfo&);

#endif
