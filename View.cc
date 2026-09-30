#include "View.h"
#include <algorithm>

using namespace std;

void View::drawJarFrame(const std::vector<MarbleRGB>& marbles, int startOffset, int count) const {
    const int visibleMarbles = min(jarCapacity, count);

    // Top Lid
    cout << "         .-------------.\n";

    // Top Curve Row (6 marbles: slots 94 to 99)
    cout << "         / ";
    for (int col = 0; col < 6; ++col) {
        if (col > 0) cout << " ";
        const int slotIndex = 94 + col;
        if (slotIndex < visibleMarbles) {
            const MarbleRGB& marble = marbles[startOffset + slotIndex];
            cout << "\033[38;2;" << marble.r << ";" << marble.g << ";" << marble.b << "mo\033[0m";
        } else {
            cout << " ";
        }
    }
    cout << " \\\n";

    // Middle 11x8 Grid (88 marbles: slots 6 to 93, top to bottom)
    for (int row = 10; row >= 0; --row) {
        cout << "        |";
        for (int col = 0; col < 8; ++col) {
            if (col > 0) cout << " ";
            const int slotIndex = 6 + (row * 8) + col;
            if (slotIndex < visibleMarbles) {
                const MarbleRGB& marble = marbles[startOffset + slotIndex];
                cout << "\033[38;2;" << marble.r << ";" << marble.g << ";" << marble.b << "mo\033[0m";
            } else {
                cout << " ";
            }
        }
        cout << "|\n";
    }

    // Bottom Curve Row (6 marbles: slots 0 to 5)
    cout << "         \\ ";
    for (int col = 0; col < 6; ++col) {
        if (col > 0) cout << " ";
        const int slotIndex = col;
        if (slotIndex < visibleMarbles) {
            const MarbleRGB& marble = marbles[startOffset + slotIndex];
            cout << "\033[38;2;" << marble.r << ";" << marble.g << ";" << marble.b << "mo\033[0m";
        } else {
            cout << " ";
        }
    }
    cout << " /\n";

    // Flat Base
    cout << "         '-------------'\n";
}

void View::displayMarbleJar(const std::vector<MarbleRGB>& marbles) const {
    const int totalMarbles = static_cast<int>(marbles.size());
    const int fullJars = totalMarbles / jarCapacity;
    const int activeJarCount = totalMarbles % jarCapacity;
    const int activeJarOffset = fullJars * jarCapacity;

    drawJarFrame(marbles, activeJarOffset, activeJarCount);

    cout << "          " << activeJarCount << " marbles in active jar\n";
    if (fullJars > 0) {
        std::string suffix = "s";
        if (fullJars == 1) {
            suffix = "";
        }
        cout << "          + " << fullJars << " full jar" << suffix 
             << " (" << totalMarbles << " total marbles)\n";
    } else {
        cout << "          (" << totalMarbles << " total marbles)\n";
    }
}

void View::displayAllJars(const std::vector<MarbleRGB>& marbles) const {
    const int totalMarbles = static_cast<int>(marbles.size());
    if (totalMarbles == 0) {
        displayMarbleJar(marbles);
        return;
    }

    const int fullJars = totalMarbles / jarCapacity;
    const int activeJarCount = totalMarbles % jarCapacity;

    for (int i = 0; i < fullJars; ++i) {
        cout << "\n--- JAR " << (i + 1) << " (FULL: " << jarCapacity << "/" << jarCapacity << ") ---\n";
        drawJarFrame(marbles, i * jarCapacity, jarCapacity);
    }

    if (activeJarCount > 0 || fullJars == 0) {
        cout << "\n--- ACTIVE JAR (" << activeJarCount << "/" << jarCapacity << ") ---\n";
        drawJarFrame(marbles, fullJars * jarCapacity, activeJarCount);
    }

    cout << "\nTotal All-Time Marbles: " << totalMarbles << "\n";
}

void View::showHomeScreen(const std::vector<MarbleRGB>& marbles) const {
    displayMarbleJar(marbles);
    displayHelpMenu();
}

void View::displayHelpMenu() const {
    cout << "\ncommands:\n";
    cout << " marbles -log                             log a task\n";
    cout << " marbles -tasks                           view all tasks\n";
    cout << " marbles -delete -TASK_NAME              remove a task\n";
    cout << " marbles -jar                             view active marble jar\n";
    cout << " marbles -jars                            view all full & active jars\n";
    cout << " marbles -history                         view previous days\n";
    cout << " marbles -history -DD/MM/YY              view one day's history\n";
    cout << "------------------------------------------------------------\n";
}

void View::displayMessage(const string& message) const {
    cout << "[MARBLES] " << message << endl;
}

void View::displayTasks(const std::vector<ActionReward>& habits) const {
    if (habits.empty()) {
        cout << "no actions added" << endl;
        return;
    }

    for (const ActionReward& h : habits) {
        std::string repeatFlagStr = "n";
        if (h.repeatable) {
            repeatFlagStr = "y";
        }

        cout << h.name << ": " << h.worth << " marbles (repeatable: " << repeatFlagStr << ")";
        if (h.repeatable) {
            cout << " (repeat: " << h.repeatWorth << " marbles)";
        }
        cout << endl;
    }
}

void View::displayHistorySummary(const std::vector<DailyLog>& logs) const {
    if (logs.empty()) {
        cout << "no history available" << endl;
        return;
    }

    for (const DailyLog& log : logs) {
        cout << log.date << ": " << log.total << " marbles" << endl;
    }
}

void View::displayHistoryForDate(const std::string& userDate, const DailyLog* log) const {
    if (log == nullptr) {
        cout << "no history for " << userDate << endl;
        return;
    }

    cout << userDate << ": " << log->total << " marbles" << endl;
    if (!log->tasks.empty()) {
        cout << "tasks:" << endl;
        for (const TaskEntry& task : log->tasks) {
            cout << " " << task.name << ": " << task.worth << " marbles" << endl;
        }
    }
}