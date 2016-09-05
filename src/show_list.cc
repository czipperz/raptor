#include "show_list.hh"
#include <iostream>

void show_list(const std::vector<std::string>& list) {
    if (list.size()) {
        typedef std::vector<std::string>::const_iterator itrtype;
        itrtype itr = list.begin();
        std::cout << *itr;
        while (++itr != list.end()) {
            std::cout << ", " << *itr;
        }
        std::cout << std::endl;
    }
}
