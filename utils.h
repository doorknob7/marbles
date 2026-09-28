#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace utils {
    std::string removeLeadingDash(const char* argument);
    std::string currentDate();
    bool convertDate(const std::string& input, std::string& storedDate);
}

#endif