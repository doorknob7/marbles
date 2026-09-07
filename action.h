#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class action {

    private:
        

    public:
        action(){};
        void registerAction();
        void deregisterAction();
        void alterActionReward();
        void logAction();
        void viewActionsWithRewards();
        void viewMarbleJar();
        void recallByDate();
        void getWeeklySummary();
        void deleteLoggedAction();


};

#endif