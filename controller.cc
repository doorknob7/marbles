#include "controller.h"
#include <iostream>

int controller::run() {
    std::cout << "controller running!" << std::endl;
    
    marbles marble;
    marble.handleMarbleFlags();

    action action;
    action.registerAction();

    storage storage;
    storage.saveData();
    return 0;
}