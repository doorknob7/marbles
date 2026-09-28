#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class view {
private:
    static constexpr int jarCapacity = 100; //6 top row, 6 bottom row, 11 rows of 8 in middle (6+6+88=100)

    // Helper method to draw a single jar at a specific starting offset
    void drawJarFrame(const nlohmann::json& marbles, int startOffset, int count) const;

public:
    void displayMarbleJar(const nlohmann::json& marbles = nlohmann::json::array()) const;
    void displayAllJars(const nlohmann::json& marbles = nlohmann::json::array()) const;
    void showHomeScreen(const nlohmann::json& marbles = nlohmann::json::array()) const;
    void displayHelpMenu() const;
    void displayMessage(const std::string& message) const;
};

#endif