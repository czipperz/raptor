#include "fix_optdepends.hh"
#include <algorithm>

void fix_optdepends(srcinfo& info) {
    for (std::vector<std::string>::iterator vitr =
             info.optdepends.begin();
         vitr != info.optdepends.end(); ++vitr) {
        std::string::iterator sitr = std::find(vitr->begin(),
                                               vitr->end(),
                                               ':');
        vitr->erase(sitr, vitr->end());
    }
}
