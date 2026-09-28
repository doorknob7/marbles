#include "utils.h"
#include <ctime>
#include <stdexcept>

namespace utils {

std::string removeLeadingDash(const char* argument) {
    if (!argument) return "";
    std::string value(argument);
    if (!value.empty() && value.front() == '-') {
        value.erase(0, 1);
    }
    return value;
}

std::string currentDate() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_r(&now, &localTime);

    char date[11];
    std::strftime(date, sizeof(date), "%Y-%m-%d", &localTime);
    return date;
}

bool convertDate(const std::string& input, std::string& storedDate) {
    if (input.size() != 8 || input[2] != '/' || input[5] != '/') {
        return false;
    }

    const std::string day = input.substr(0, 2);
    const std::string month = input.substr(3, 2);
    const std::string shortYear = input.substr(6, 2);
    try {
        const int dayValue = std::stoi(day);
        const int monthValue = std::stoi(month);
        const int yearValue = std::stoi(shortYear);
        if (dayValue < 1 || dayValue > 31 || monthValue < 1 || monthValue > 12) {
            return false;
        }
        storedDate = "20" + shortYear + "-" + month + "-" + day;
        return yearValue >= 0;
    } catch (const std::exception&) {
        return false;
    }
}

} // namespace utils