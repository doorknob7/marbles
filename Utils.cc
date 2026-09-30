#include "Utils.h"
#include <ctime>
#include <stdexcept>

namespace utils {

// Strips leading dash from argument strings
std::string removeLeadingDash(const char* argument) {
    if (!argument) {
        return "";
    }
    std::string value(argument);
    if (!value.empty() && value.front() == '-') {
        value.erase(0, 1);
    }
    return value;
}

// Combines multi-word arguments into a single space-separated string
std::string combineArgs(int argc, char* argv[], int startIdx) {
    if (!argv || startIdx >= argc) {
        return "";
    }

    std::string combined = removeLeadingDash(argv[startIdx]);
    for (int i = startIdx + 1; i < argc; ++i) {
        combined += " ";
        combined += argv[i];
    }
    return combined;
}

// Generates current calendar date string in YYYY-MM-DD format
std::string currentDate() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_r(&now, &localTime);

    char dateBuffer[11];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localTime);
    return dateBuffer;
}

// Validates DD/MM/YY string format and converts to ISO YYYY-MM-DD
bool convertDate(const std::string& input, std::string& storedDate) {
    // Clean leading dash if present (e.g., "-24/09/26")
    std::string cleanInput = (!input.empty() && input.front() == '-') ? input.substr(1) : input;

    if (cleanInput.size() != 8 || cleanInput[2] != '/' || cleanInput[5] != '/') {
        return false;
    }

    const std::string dayStr = cleanInput.substr(0, 2);
    const std::string monthStr = cleanInput.substr(3, 2);
    const std::string shortYearStr = cleanInput.substr(6, 2);

    try {
        const int dayValue = std::stoi(dayStr);
        const int monthValue = std::stoi(monthStr);
        const int yearValue = std::stoi(shortYearStr);

        // Validate day and month calendar ranges
        if (dayValue < 1 || dayValue > 31 || monthValue < 1 || monthValue > 12 || yearValue < 0) {
            return false;
        }

        storedDate = "20" + shortYearStr + "-" + monthStr + "-" + dayStr;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

} // namespace utils