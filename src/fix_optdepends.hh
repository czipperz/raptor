#ifndef HEADER_GUARD_FIX_OPTDEPENDS_H
#define HEADER_GUARD_FIX_OPTDEPENDS_H

#include "srcinfo_parser.hh"

/// Takes optdepends that look like
///     aurvote: vote for favorite packages from the AUR
/// and turns them into
///     aurvote
void fix_optdepends(srcinfo&);

#endif
