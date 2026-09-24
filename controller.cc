#include "controller.h"
#include <iostream>
using namespace std;

int controller::run(int argc, char* argv[]) {
    
    view view;
    marbles marble;
    action action;
    storage storage;
    action.loadFromJson(storage.loadData());

    if ((argc == 1) || (string(argv[1]) == "help")) {
        view.showHomeScreen(action.currentDayMarbles());
    } else if(string(argv[1])=="-log"){
        action.registerAction(argc, argv);
    } else if(string(argv[1])=="-tasks"){
        action.viewActionsWithRewards();
    } else if(string(argv[1])=="-delete"){
        action.deleteAction(argc, argv);
    } else if(string(argv[1])=="-jar"){
        view.displayMarbleJar(action.currentDayMarbles());
    } else if(string(argv[1])=="-history"){
        if (argc >= 3) {
            action.recallByDate(argv[2]);
        } else {
            action.recallByDate();
        }
    } else {
        cout << "invalid command" << endl;
        view.displayHelpMenu();
    }
    
    /*for (int i = 1; i < argc; ++i) {
            cout << "  argv[" << i << "]: " << argv[i] << std::endl;
    }*/

    return 0;
}