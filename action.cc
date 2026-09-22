#include "action.h"
#include "storage.h"
#include <stdexcept>

namespace {
std::string removeLeadingDash(const char* argument) {
    std::string value(argument);
    if (!value.empty() && value.front() == '-') {
        value.erase(0, 1);
    }
    return value;
}
}

void action::registerAction(int argc, char* argv[]) {
    if (argc < 5 || std::string(argv[2]) != "-add") {
        std::cout << "usage: marbles -log -add -NAME -WORTH (in decimal) -REPEATABLE (y/n) -[REPEAT_WORTH] (OPTIONAL - only if subsequent logs of same action have a decreased value)" << std::endl;
        return;
    }

    try {
        const bool repeatable = argc >= 6 &&
            (removeLeadingDash(argv[5]) == "true" ||
             removeLeadingDash(argv[5]) == "1" ||
             removeLeadingDash(argv[5]) == "y" ||
             removeLeadingDash(argv[5]) == "Y");
        ActionReward actionReward{
            removeLeadingDash(argv[3]),
            std::stod(removeLeadingDash(argv[4])),
            repeatable,
            0.0
        };

        if (actionReward.repeatable) {
            if (argc < 7) {
                std::cout << "a repeatable action needs a repeat worth" << std::endl;
                return;
            }
            actionReward.repeatWorth = std::stod(removeLeadingDash(argv[6]));
        }

        actionRewards.push_back(actionReward);
        storage dataStorage;
        dataStorage.saveData(toJson());
        viewActionsWithRewards();
    } catch (const std::invalid_argument&) {
        std::cout << "worth values must be numbers" << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "worth value is out of range" << std::endl;
    }
}

void action::viewActionsWithRewards() {
    if (actionRewards.empty()) {
        std::cout << "no actions added" << std::endl;
        return;
    }

    for (const ActionReward& actionReward : actionRewards) {
        std::cout << actionReward.name << ": " << actionReward.worth
                  << " marbles (repeatable: " << (actionReward.repeatable ? "y" : "n") << ")";
        if (actionReward.repeatable) {
            std::cout << " (repeat: " << actionReward.repeatWorth << " marbles)";
        }
        std::cout << std::endl;
    }
}

void action::recallByDate() {
    std::cout << "recalling by date!" << std::endl;
}

nlohmann::json action::toJson() const {
    nlohmann::json data{
        {"habits", nlohmann::json::array()},
        {"logs", nlohmann::json::array()}
    };

    for (const ActionReward& actionReward : actionRewards) {
        data["habits"].push_back({
            {"name", actionReward.name},
            {"reward", actionReward.worth},
            {"repeatable", actionReward.repeatable},
            {"repeatWorth", actionReward.repeatWorth}
        });
    }
    return data;
}

void action::loadFromJson(const nlohmann::json& data) {
    actionRewards.clear();
    if (!data.contains("habits") || !data["habits"].is_array()) {
        return;
    }

    for (const nlohmann::json& habit : data["habits"]) {
        actionRewards.push_back({
            habit.value("name", ""),
            habit.value("reward", 0.0),
            habit.value("repeatable", false),
            habit.value("repeatWorth", 0.0)
        });
    }
}