#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <string>

class view {

    private:
        

    public:
        void displayMarbleJar() const;
        void showHomeScreen() const;
        void displayHelpMenu() const;
        void displayMessage(const std::string& message) const;


};

#endif