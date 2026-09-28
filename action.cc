#include "action.h"
#include "utils.h"
#include "marbles.h"
#include "storage.h"
#include <algorithm>
#include <cmath>

void action::registerAction(int argc, char* argv[]) {
    if (argc >= 3 && std::string(argv[2]) != "-add") {
        logAction(argc, argv);
        return;
    }

    if (argc < 5 || std::string(argv[2]) != "-add") {
        std::cout << "usage: marbles -log -add -NAME -WORTH [-Y/-N] [REPEAT_WORTH]" << std::endl;
        return;
    }

    try {
        const bool repeatable = argc >= 6 &&
            (utils::removeLeadingDash(argv[5]) == "true" ||
             utils::removeLeadingDash(argv[5]) == "1" ||
             utils::removeLeadingDash(argv[5]) == "y" ||
             utils::removeLeadingDash(argv[5]) == "Y");

        ActionReward actionReward{
            utils::removeLeadingDash(argv[3]),
            std::stod(utils::removeLeadingDash(argv[4])),
            repeatable,
            0.0
        };

        if (actionReward.repeatable && argc >= 7) {
            actionReward.repeatWorth = std::stod(utils::removeLeadingDash(argv[6]));
        }

        actionRewards.push_back(actionReward);
        storage dataStorage;
        dataStorage.saveData(toJson());
        viewActionsWithRewards();
    } catch (const std::exception&) {
        std::cout << "invalid worth value numbers" << std::endl;
    }
}

void action::logAction(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: marbles -log -TASK_NAME" << std::endl;
        return;
    }

    std::string taskName = utils::removeLeadingDash(argv[2]);
    for (int i = 3; i < argc; ++i) {
        taskName += " ";
        taskName += argv[i];
    }

    const auto task = std::find_if(
        actionRewards.begin(), actionRewards.end(),
        [&taskName](const ActionReward& r) { return r.name == taskName; });

    if (task == actionRewards.end()) {
        std::cout << "task not found: " << taskName << std::endl;
        return;
    }

    const std::string date = utils::currentDate();
    nlohmann::json* dailyLog = nullptr;
    for (nlohmann::json& log : logs) {
        if (log.value("date", "") == date) {
            dailyLog = &log;
            break;
        }
    }

    if (dailyLog == nullptr) {
        logs.push_back({
            {"date", date},
            {"tasks", nlohmann::json::array()},
            {"marbles", nlohmann::json::array()},
            {"total", 0.0}
        });
        dailyLog = &logs.back();
    }

    std::size_t loggedCount = 0;
    for (const nlohmann::json& loggedTask : (*dailyLog)["tasks"]) {
        if (loggedTask.value("name", "") == taskName) {
            ++loggedCount;
        }
    }

    if (!task->repeatable && loggedCount > 0) {
        std::cout << "task already logged today: " << taskName << std::endl;
        return;
    }

    const double previousTotal = dailyLog->value("total", 0.0);
    const double worth = (loggedCount > 0 && task->repeatWorth > 0.0)
        ? task->repeatWorth
        : task->worth;

    (*dailyLog)["tasks"].push_back({{"name", taskName}, {"worth", worth}});
    (*dailyLog)["total"] = previousTotal + worth;

    const int previousMarbles = static_cast<int>(std::floor(previousTotal + 1e-9));
    const int currentMarbles = static_cast<int>(std::floor(previousTotal + worth + 1e-9));

    for (int m = previousMarbles; m < currentMarbles; ++m) {
        (*dailyLog)["marbles"].push_back(marbles::randomMarble());
        std::cout << "a marble has been added!" << std::endl;
    }

    storage dataStorage;
    dataStorage.saveData(toJson());
    std::cout << "logged " << taskName << ": " << worth << " marbles" << std::endl;
}

void action::deleteAction(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: marbles -delete -TASK_NAME" << std::endl;
        return;
    }

    std::string taskName = utils::removeLeadingDash(argv[2]);
    for (int i = 3; i < argc; ++i) {
        taskName += " ";
        taskName += argv[i];
    }

    const auto origSize = actionRewards.size();
    actionRewards.erase(
        std::remove_if(actionRewards.begin(), actionRewards.end(),
                       [&taskName](const ActionReward& r) { return r.name == taskName; }),
        actionRewards.end());

    if (actionRewards.size() == origSize) {
        std::cout << "task not found: " << taskName << std::endl;
        return;
    }

    storage dataStorage;
    dataStorage.saveData(toJson());
    std::cout << "deleted task: " << taskName << std::endl;
}

void action::viewActionsWithRewards() const {
    if (actionRewards.empty()) {
        std::cout << "no actions added" << std::endl;
        return;
    }

    for (const ActionReward& r : actionRewards) {
        std::cout << r.name << ": " << r.worth << " marbles (repeatable: " 
                  << (r.repeatable ? "y" : "n") << ")";
        if (r.repeatable) {
            std::cout << " (repeat: " << r.repeatWorth << " marbles)";
        }
        std::cout << std::endl;
    }
}

void action::recallByDate() const {
    if (logs.empty()) {
        std::cout << "no history available" << std::endl;
        return;
    }

    for (const nlohmann::json& log : logs) {
        std::cout << log.value("date", "unknown date") << ": "
                  << log.value("total", 0.0) << " marbles" << std::endl;
    }
}

void action::recallByDate(const std::string& date) const {
    const std::string requestedDate = (!date.empty() && date.front() == '-') ? date.substr(1) : date;
    std::string storedDate;
    if (!utils::convertDate(requestedDate, storedDate)) {
        std::cout << "invalid date: " << date << " (use DD/MM/YY)" << std::endl;
        return;
    }

    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") == storedDate) {
            std::cout << requestedDate << ": " << log.value("total", 0.0) << " marbles" << std::endl;
            if (log.contains("tasks") && log["tasks"].is_array()) {
                std::cout << "tasks:" << std::endl;
                for (const nlohmann::json& t : log["tasks"]) {
                    std::cout << " " << t.value("name", "unknown") << ": " 
                              << t.value("worth", 0.0) << " marbles" << std::endl;
                }
            }
            return;
        }
    }

    std::cout << "no history for " << requestedDate << std::endl;
}

nlohmann::json action::toJson() const {
    nlohmann::json data{{"habits", nlohmann::json::array()}, {"logs", logs}};
    for (const ActionReward& r : actionRewards) {
        data["habits"].push_back({
            {"name", r.name},
            {"reward", r.worth},
            {"repeatable", r.repeatable},
            {"repeatWorth", r.repeatWorth}
        });
    }
    return data;
}

void action::loadFromJson(const nlohmann::json& data) {
    actionRewards.clear();
    logs = data.value("logs", nlohmann::json::array());

    if (data.contains("habits") && data["habits"].is_array()) {
        for (const nlohmann::json& h : data["habits"]) {
            actionRewards.push_back({
                h.value("name", ""),
                h.value("reward", 0.0),
                h.value("repeatable", false),
                h.value("repeatWorth", 0.0)
            });
        }
    }
}