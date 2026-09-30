#include "Controller.h"
#include "Action.h"
#include "Marbles.h"
#include "Storage.h"
#include "View.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int Controller::run(int argc, char* argv[]) {
    View viewManager;
    Storage storageManager;
    Action actionManager;
    Marbles marbleManager;

    // Load persistent data into memory
    storageManager.loadData(actionManager);

    // Compute cumulative all-time marbles vector
    std::vector<MarbleRGB> allMarbles = marbleManager.totalMarbles(actionManager.getLogs());

    if (argc == 1) {
        // Default execution displays home screen
        viewManager.showHomeScreen(allMarbles);
        return 0;
    }

    string cmd = argv[1];

    if (cmd == "help" || cmd == "-help" || cmd == "--help") {
        viewManager.displayHelpMenu();
    } 
    else if (cmd == "-log") {
        if (argc >= 3 && string(argv[2]) == "-add") {
            if (argc < 5) {
                cout << "usage: marbles -log -add -NAME -WORTH [-Y/-N] [REPEAT_WORTH]" << endl;
                return 1;
            }

            string name = utils::removeLeadingDash(argv[3]);
            
            // Safely parse numeric arguments to prevent exceptions on invalid input
            try {
                double worth = stod(utils::removeLeadingDash(argv[4]));
                
                // Clean repeatable boolean parsing
                bool repeatable = false;
                if (argc >= 6) {
                    string repeatFlag = utils::removeLeadingDash(argv[5]);
                    if (repeatFlag == "true" || repeatFlag == "1" || repeatFlag == "y" || repeatFlag == "Y") {
                        repeatable = true;
                    }
                }

                double repeatWorth = 0.0;
                if (repeatable && argc >= 7) {
                    repeatWorth = stod(utils::removeLeadingDash(argv[6]));
                }

                if (actionManager.addHabit(name, worth, repeatable, repeatWorth)) {
                    storageManager.saveData(actionManager);
                    viewManager.displayTasks(actionManager.getHabits());
                } else {
                    cout << "failed to add habit (duplicate name, empty name, or negative worth value)" << endl;
                }
            } catch (const std::exception&) {
                cout << "error: habit worth values must be valid numeric numbers" << endl;
                return 1;
            }
        } 
        else if (argc >= 3) {
            string taskName = utils::combineArgs(argc, argv, 2);
            int marblesAdded = 0;

            Action::LogStatus status = actionManager.logTask(taskName, marblesAdded);
            if (status == Action::LogStatus::SUCCESS) {
                storageManager.saveData(actionManager);
                for (int i = 0; i < marblesAdded; ++i) {
                    cout << "a marble has been added!" << endl;
                }
                cout << "logged " << taskName << endl;
            } else if (status == Action::LogStatus::TASK_NOT_FOUND) {
                cout << "task not found: " << taskName << endl;
            } else if (status == Action::LogStatus::ALREADY_LOGGED_TODAY) {
                cout << "task already logged today: " << taskName << endl;
            }
        } 
        else {
            cout << "usage: marbles -log -TASK_NAME" << endl;
        }
    } 
    else if (cmd == "-tasks") {
        viewManager.displayTasks(actionManager.getHabits());
    } 
    else if (cmd == "-delete") {
        if (argc < 3) {
            cout << "usage: marbles -delete -TASK_NAME" << endl;
            return 1;
        }
        string taskName = utils::combineArgs(argc, argv, 2);
        if (actionManager.deleteHabit(taskName)) {
            storageManager.saveData(actionManager);
            cout << "deleted task: " << taskName << endl;
        } else {
            cout << "task not found: " << taskName << endl;
        }
    } 
    else if (cmd == "-jar") {
        viewManager.displayMarbleJar(allMarbles);
    } 
    else if (cmd == "-jars") {
        viewManager.displayAllJars(allMarbles);
    } 
    else if (cmd == "-history") {
        if (argc >= 3) {
            string rawDate = argv[2];
            string isoDate;
            if (utils::convertDate(rawDate, isoDate)) {
                const DailyLog* log = actionManager.getLogForDate(isoDate);
                viewManager.displayHistoryForDate(rawDate, log);
            } else {
                cout << "invalid date: " << rawDate << " (use DD/MM/YY)" << endl;
            }
        } else {
            viewManager.displayHistorySummary(actionManager.getLogs());
        }
    } 
    else {
        cout << "invalid command: " << cmd << endl;
        viewManager.displayHelpMenu();
    }

    return 0;
}