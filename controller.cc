#include "controller.h"
#include <iostream>
using namespace std;

int controller::run(int argc, char* argv[]) {
    
    view view;
    action action;
    storage storage;
    marbles marble;
    
    action.loadFromJson(storage.loadData());

    // Fetch cumulative all-time marbles via the marbles class
    nlohmann::json allMarbles = marble.totalMarbles(action.getLogs());

    if ((argc == 1) || (string(argv[1]) == "help")) {
        // Home screen shows active jar from all-time cumulative history
        view.showHomeScreen(allMarbles);
    } else if (string(argv[1]) == "-log") {
        action.registerAction(argc, argv);
    } else if (string(argv[1]) == "-tasks") {
        action.viewActionsWithRewards();
    } else if (string(argv[1]) == "-delete") {
        action.deleteAction(argc, argv);
    } else if (string(argv[1]) == "-jar") {
        // -jar always renders active jar using all-time history
        view.displayMarbleJar(allMarbles);
    } else if (string(argv[1]) == "-jars") {
        // -jars renders all filled jars + active jar
        view.displayAllJars(allMarbles);
    } else if (string(argv[1]) == "-history") {
        if (argc >= 3) {
            action.recallByDate(argv[2]);
        } else {
            action.recallByDate();
        }
    } else {
        cout << "invalid command" << endl;
        view.displayHelpMenu();
    }

    return 0;
}