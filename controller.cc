#include "controller.h"
#include <iostream>

int controller::run(int argc, char* argv[]) {
    std::cout << "controller running!" << std::endl;
    
    // Test line: print all arguments passed into the CLI
    std::cout << "Received " << argc << " argument(s):" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "  argv[" << i << "]: " << argv[i] << std::endl;
    }
    
    marbles marble;
    marble.handleMarbleFlags();

    action action;
    action.registerAction();

    storage storage;
    storage.saveData();
    return 0;
}