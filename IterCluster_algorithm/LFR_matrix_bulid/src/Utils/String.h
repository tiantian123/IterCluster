#ifndef __LFR_UTILS_STRING_H__
#define __LFR_UTILS_STRING_H__
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

#endif
