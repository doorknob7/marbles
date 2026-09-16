#include "action.h"
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
    if (argc < 6 || std::string(argv[2]) != "-add") {
        std::cout << "usage: marbles -log -add NAME WORTH REPEATABLE [REPEAT_WORTH]" << std::endl;
        return;
    }

    try {
        ActionReward actionReward{
            removeLeadingDash(argv[3]),
            std::stod(removeLeadingDash(argv[4])),
            removeLeadingDash(argv[5]) == "true" || removeLeadingDash(argv[5]) == "1",
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
        viewActionsWithRewards();
    } catch (const std::invalid_argument&) {
        std::cout << "worth values must be numbers" << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "worth value is out of range" << std::endl;
    }
}

void action::viewActionsWithRewards() {
    for (const ActionReward& actionReward : actionRewards) {
        std::cout << actionReward.name << ": " << actionReward.worth << " marbles";
        if (actionReward.repeatable) {
            std::cout << " (repeat: " << actionReward.repeatWorth << " marbles)";
        }
        std::cout << std::endl;
    }
}

void action::recallByDate() {
    std::cout << "recalling by date!" << std::endl;
}