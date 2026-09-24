#include "view.h"
#include <algorithm>

using namespace std;

void view::displayMarbleJar(const nlohmann::json& marbles) const {
    static constexpr int jarWidth = 25;
    static constexpr int jarHeight = 8;
    const int marbleTotal = min(
        jarWidth * jarHeight,
        marbles.is_array() ? static_cast<int>(marbles.size()) : 0);

    cout << "          .------------.\n";
    cout << "         /              \\\n";
    for (int row = 0; row < jarHeight; ++row) {
        cout << "        | ";
        for (int column = 0; column < jarWidth; ++column) {
            const int marbleIndex = (jarHeight - 1 - row) * jarWidth + column;
            if (marbleIndex < marbleTotal) {
                const nlohmann::json& marble = marbles[marbleIndex];
                cout << "\033[38;2;" << marble.value("r", 120) << ";"
                     << marble.value("g", 120) << ";" << marble.value("b", 120)
                     << "m o\033[0m";
            } else {
                cout << "  ";
            }
        }
        cout << " |\n";
    }
    cout << "         \\______________/\n";
    cout << "          " << marbleTotal << " marbles today\n";
}

void view::showHomeScreen(const nlohmann::json& marbles) const {

    displayMarbleJar(marbles);

    cout << "\n";
    cout << "commands:\n";
    cout << " marbles -log                             log a task\n";
    cout << " marbles -tasks                           view all tasks\n";
    cout << " marbles -delete -TASK_NAME              remove a task\n";
    cout << " marbles -jar                             view marble jar\n";
    cout << " marbles -history                         view previous days\n";
    cout << " marbles -history -DD/MM/YY              view one day's history\n";
    cout << "------------------------------------------------------------\n";
}
//add <name> <value, in fraction>       add a new habit\n
void view::displayHelpMenu() const {
    showHomeScreen();
}

void view::displayMessage(const string& message) const {
    cout << "[MARBLES] " << message << endl;
}