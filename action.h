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
        nlohmann::json logs = nlohmann::json::array();

    public:
        action(){};
        void registerAction(int argc, char* argv[]);
        void deregisterAction();
        void deleteAction(int argc, char* argv[]);
        void alterActionReward();
        void logAction(int argc, char* argv[]);
        void viewActionsWithRewards();
        void viewMarbleJar();
        double currentDayTotal() const;
        nlohmann::json currentDayMarbles() const;
        void recallByDate();
        void recallByDate(const std::string& date);
        void getWeeklySummary();
        void deleteLoggedAction();
        nlohmann::json toJson() const;
        void loadFromJson(const nlohmann::json& data);


};

#endif