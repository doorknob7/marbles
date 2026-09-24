#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class view {

    private:
        

    public:
        void displayMarbleJar(const nlohmann::json& marbles = nlohmann::json::array()) const;
        void showHomeScreen(const nlohmann::json& marbles = nlohmann::json::array()) const;
        void displayHelpMenu() const;
        void displayMessage(const std::string& message) const;


};

#endif