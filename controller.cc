#include "controller.h"
#include <iostream>

int controller::run(int argc, char* argv[]) {
    std::cout << "controller running!" << std::endl;
    view view;
    
    // Test line: print all arguments passed into the CLI
    std::cout << "Received " << argc << " argument(s):" << std::endl;

    if ((argc == 1) || (argc == 2 && std::string(argv[1]) == "help")) {
        view.showHomeScreen();    
    } else {
        for (int i = 1; i < argc; ++i) {
            std::cout << "  argv[" << i << "]: " << argv[i] << std::endl;
        }
    }
    
    marbles marble;
    marble.handleMarbleFlags();

    action action;
    action.registerAction();

    storage storage;
    storage.saveData();

    return 0;
}