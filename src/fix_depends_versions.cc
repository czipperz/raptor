#include "fix_depends_versions.hh"

///     libc6>=2.15
/// to
///     libc6 (>= 2.15)
/// returns true if string was changed
static bool gr_eq(std::string& str) {
    for (const char* sitr = str.c_str(); *sitr; ++sitr) {
        if (sitr[0] == '>' and sitr[1] == '=') {
            // libc6>=2.15
            size_t index = sitr - str.c_str();
            // libc6>= 2.15
            str.insert(index + 2, " ");
            // libc6 (>= 2.15
            str.insert(index, " (");
            // libc6 (>= 2.15)
            str.push_back(')');
            return true;
        }
    }
    return false;
}

// ///     libc6=2.15
// /// to
// ///     libc6 (2.15)
// /// returns true if string was changed
// static bool eq(std::string& str) {
//     for (std::string::iterator sitr = str.begin(); sitr != str.end();
//          ++sitr) {
//     }
//     return false;
// }

static void fix_each(std::vector<std::string>& vec) {
    for (std::vector<std::string>::iterator vitr = vec.begin();
         vitr != vec.end(); ++vitr) {
        gr_eq(*vitr) /* || eq(*vitr) */;
    }
}

void fix_depends_versions(srcinfo& info) {
    fix_each(info.depends);
    fix_each(info.optdepends);
}
