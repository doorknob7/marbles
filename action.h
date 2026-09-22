#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class action {

    private:
        struct ActionReward {
            std::string name;
            double worth;
            bool repeatable;
            double repeatWorth;
        };

        std::vector<ActionReward> actionRewards;

    public:
        action(){};
        void registerAction(int argc, char* argv[]);
        void deregisterAction();
        void alterActionReward();
        void logAction();
        void viewActionsWithRewards();
        void viewMarbleJar();
        void recallByDate();
        void getWeeklySummary();
        void deleteLoggedAction();
        nlohmann::json toJson() const;
        void loadFromJson(const nlohmann::json& data);


};

#endif