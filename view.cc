#include "view.h"
#include <algorithm>

using namespace std;

void view::drawJarFrame(const nlohmann::json& marbles, int startOffset, int count) const {
    
    const int visibleMarbles = min(jarCapacity, count);
    //jar lid
    cout << "         .-------------.\n";

    // 2. Top Curve Row (6 marbles: slots 94 to 99)
    cout << "         / ";
    for (int col = 0; col < 6; ++col) {
        if (col > 0) cout << " ";
        const int slotIndex = 94 + col;
        if (slotIndex < visibleMarbles) {
            const auto& marble = marbles[startOffset + slotIndex];
            cout << "\033[38;2;" << marble.value("r", 120) << ";"
                 << marble.value("g", 120) << ";" << marble.value("b", 120)
                 << "mo\033[0m";
        } else {
            cout << " ";
        }
    }
    cout << " \\\n";

    // 3. Middle 11x8 Grid (88 marbles: slots 6 to 93, rendered top-to-bottom)
    for (int row = 10; row >= 0; --row) {
        cout << "        |";
        for (int col = 0; col < 8; ++col) {
            if (col > 0) cout << " ";
            const int slotIndex = 6 + (row * 8) + col;
            if (slotIndex < visibleMarbles) {
                const auto& marble = marbles[startOffset + slotIndex];
                cout << "\033[38;2;" << marble.value("r", 120) << ";"
                     << marble.value("g", 120) << ";" << marble.value("b", 120)
                     << "mo\033[0m";
            } else {
                cout << " ";
            }
        }
        cout << "|\n";
    }

    // 4. Bottom Curve Row (6 marbles: slots 0 to 5)
    cout << "         \\ ";
    for (int col = 0; col < 6; ++col) {
        if (col > 0) cout << " ";
        const int slotIndex = col;
        if (slotIndex < visibleMarbles) {
            const auto& marble = marbles[startOffset + slotIndex];
            cout << "\033[38;2;" << marble.value("r", 120) << ";"
                 << marble.value("g", 120) << ";" << marble.value("b", 120)
                 << "mo\033[0m";
        } else {
            cout << " ";
        }
    }
    cout << " /\n";

    // 5. Flat Base
    cout << "         '-------------'\n";
}

void view::displayMarbleJar(const nlohmann::json& marbles) const {
    const int totalMarbles = marbles.is_array() ? static_cast<int>(marbles.size()) : 0;
    const int fullJars = totalMarbles / jarCapacity;
    const int activeJarCount = totalMarbles % jarCapacity;
    const int activeJarOffset = fullJars * jarCapacity;

    // Draw active jar using all-time marble offset
    drawJarFrame(marbles, activeJarOffset, activeJarCount);

    // Cumulative stats
    cout << "          " << activeJarCount << " marbles in active jar\n";
    if (fullJars > 0) {
        cout << "          + " << fullJars << " full jar" << (fullJars > 1 ? "s" : "") 
             << " (" << totalMarbles << " total marbles)\n";
    } else {
        cout << "          (" << totalMarbles << " total marbles)\n";
    }
}

void view::displayAllJars(const nlohmann::json& marbles) const {
    const int totalMarbles = marbles.is_array() ? static_cast<int>(marbles.size()) : 0;
    if (totalMarbles == 0) {
        displayMarbleJar(marbles);
        return;
    }

    const int fullJars = totalMarbles / jarCapacity;
    const int activeJarCount = totalMarbles % jarCapacity;

    // Render every filled jar sequentially
    for (int i = 0; i < fullJars; ++i) {
        cout << "\n--- JAR " << (i + 1) << " (FULL: " << jarCapacity << "/" << jarCapacity << ") ---\n";
        drawJarFrame(marbles, i * jarCapacity, jarCapacity);
    }

    // Render active jar
    if (activeJarCount > 0 || fullJars == 0) {
        cout << "\n--- ACTIVE JAR (" << activeJarCount << "/" << jarCapacity << ") ---\n";
        drawJarFrame(marbles, fullJars * jarCapacity, activeJarCount);
    }
    
    cout << "\nTotal All-Time Marbles: " << totalMarbles << "\n";
}

void view::showHomeScreen(const nlohmann::json& marbles) const {
    displayMarbleJar(marbles);

    cout << "\n";
    cout << "commands:\n";
    cout << " marbles -log                             log a task\n";
    cout << " marbles -tasks                           view all tasks\n";
    cout << " marbles -delete -TASK_NAME              remove a task\n";
    cout << " marbles -jar                             view active marble jar\n";
    cout << " marbles -jars                            view all full & active jars\n";
    cout << " marbles -history                         view previous days\n";
    cout << " marbles -history -DD/MM/YY              view one day's history\n";
    cout << "------------------------------------------------------------\n";
}

void view::displayHelpMenu() const {
    showHomeScreen();
}

void view::displayMessage(const string& message) const {
    cout << "[MARBLES] " << message << endl;
}