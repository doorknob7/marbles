#include "view.h"

using namespace std;

void view::displayMarbleJar() const {
    // placeholder
    cout << "      [ MARBLE JAR PLACEHOLDER ]\n";
}

void view::showHomeScreen() const {

    displayMarbleJar();

    cout << "\n";
    cout << "commands:\n";
    cout << " marbles -log                             log a task\n";
    cout << " marbles -jar                             view marble jar\n";
    cout << " marbles -history                         view previous days\n";
    cout << "------------------------------------------------------------\n";
}
//add <name> <value, in fraction>       add a new habit\n
void view::displayHelpMenu() const {
    showHomeScreen();
}

void view::displayMessage(const string& message) const {
    cout << "[MARBLES] " << message << endl;
}