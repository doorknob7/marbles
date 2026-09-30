#ifndef UTILS_H
#define UTILS_H

#include <string>


/**
 * @namespace utils
 * @brief Utility namespace providing text processing and date parsing helpers.
 */
namespace utils {

    /**
     * @brief Strips a leading dash ('-') from a command argument string if present.
     * @param argument Raw C-style string argument.
     * @return std::string Argument string with leading dash removed.
     */
    std::string removeLeadingDash(const char* argument);

    /**
     * @brief Combines command-line arguments from a starting index into a single space-separated string.
     *        Automatically strips any leading dash from the initial argument.
     * @param argc Command-line argument count.
     * @param argv Command-line argument vector.
     * @param startIdx Index in argv from which to begin combining (default: 2).
     * @return std::string Concatenated task name or command string.
     */
    std::string combineArgs(int argc, char* argv[], int startIdx = 2);

    /**
     * @brief Retrieves the current system date formatted as YYYY-MM-DD.
     * @return std::string Formatted current date string.
     */
    std::string currentDate();

    /**
     * @brief Parses and converts a date string (e.g., "DD/MM/YY" or "-DD/MM/YY") to "YYYY-MM-DD".
     * @param input Raw date input string.
     * @param storedDate Output string populated with "YYYY-MM-DD" if conversion succeeds.
     * @return true If the date was valid and successfully converted.
     * @return false If the date format or values were invalid.
     */
    bool convertDate(const std::string& input, std::string& storedDate);

} // namespace utils

#endif