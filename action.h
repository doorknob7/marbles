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
    action() = default;

    void registerAction(int argc, char* argv[]);
    void logAction(int argc, char* argv[]);
    void deleteAction(int argc, char* argv[]);
    void viewActionsWithRewards() const;
    void recallByDate() const;
    void recallByDate(const std::string& date) const;

    // Getters for external modules
    const nlohmann::json& getLogs() const { return logs; }
    
    nlohmann::json toJson() const;
    void loadFromJson(const nlohmann::json& data);
};

#endif