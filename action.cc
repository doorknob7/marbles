#include "action.h"
#include "storage.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <stdexcept>

namespace {
std::string removeLeadingDash(const char* argument) {
    std::string value(argument);
    if (!value.empty() && value.front() == '-') {
        value.erase(0, 1);
    }
    return value;
}

std::string currentDate() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_r(&now, &localTime);

    char date[11];
    std::strftime(date, sizeof(date), "%Y-%m-%d", &localTime);
    return date;
}

bool convertDate(const std::string& input, std::string& storedDate) {
    if (input.size() != 8 || input[2] != '/' || input[5] != '/') {
        return false;
    }

    const std::string day = input.substr(0, 2);
    const std::string month = input.substr(3, 2);
    const std::string shortYear = input.substr(6, 2);
    try {
        const int dayValue = std::stoi(day);
        const int monthValue = std::stoi(month);
        const int yearValue = std::stoi(shortYear);
        if (dayValue < 1 || dayValue > 31 || monthValue < 1 || monthValue > 12) {
            return false;
        }
        storedDate = "20" + shortYear + "-" + month + "-" + day;
        return yearValue >= 0;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

nlohmann::json randomMarble() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> color(40, 215);
    return {
        {"r", color(generator)},
        {"g", color(generator)},
        {"b", color(generator)}
    };
}
}

void action::registerAction(int argc, char* argv[]) {
    if (argc >= 3 && std::string(argv[2]) != "-add") {
        logAction(argc, argv);
        return;
    }

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

        if (actionReward.repeatable && argc >= 7) {
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

void action::logAction(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: marbles -log -TASK_NAME" << std::endl;
        return;
    }

    std::string taskName = removeLeadingDash(argv[2]);
    for (int argumentIndex = 3; argumentIndex < argc; ++argumentIndex) {
        taskName += " ";
        taskName += argv[argumentIndex];
    }

    const auto task = std::find_if(
        actionRewards.begin(), actionRewards.end(),
        [&taskName](const ActionReward& actionReward) {
            return actionReward.name == taskName;
        });
    if (task == actionRewards.end()) {
        std::cout << "task not found: " << taskName << std::endl;
        return;
    }

    const std::string date = currentDate();
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

    if (!dailyLog->contains("tasks") || !(*dailyLog)["tasks"].is_array()) {
        (*dailyLog)["tasks"] = nlohmann::json::array();
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
    const double worth = loggedCount > 0 && task->repeatWorth > 0.0
        ? task->repeatWorth
        : task->worth;
    (*dailyLog)["tasks"].push_back({
        {"name", taskName},
        {"worth", worth}
    });
    (*dailyLog)["total"] = dailyLog->value("total", 0.0) + worth;

    const int previousMarbles = static_cast<int>(std::floor(previousTotal + 1e-9));
    const int currentMarbles = static_cast<int>(
        std::floor(previousTotal + worth + 1e-9));
    if (!dailyLog->contains("marbles") || !(*dailyLog)["marbles"].is_array()) {
        (*dailyLog)["marbles"] = nlohmann::json::array();
    }
    for (int marble = previousMarbles; marble < currentMarbles; ++marble) {
        (*dailyLog)["marbles"].push_back(randomMarble());
        std::cout << "a marble has been added!" << std::endl;
    }
    storage dataStorage;
    dataStorage.saveData(toJson());
    std::cout << "logged " << taskName << ": " << worth << " marbles" << std::endl;
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

void action::deleteAction(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: marbles -delete -TASK_NAME" << std::endl;
        return;
    }

    std::string taskName = removeLeadingDash(argv[2]);
    for (int argumentIndex = 3; argumentIndex < argc; ++argumentIndex) {
        taskName += " ";
        taskName += argv[argumentIndex];
    }

    const auto originalSize = actionRewards.size();
    actionRewards.erase(
        std::remove_if(actionRewards.begin(), actionRewards.end(),
                       [&taskName](const ActionReward& actionReward) {
                           return actionReward.name == taskName;
                       }),
        actionRewards.end());

    if (actionRewards.size() == originalSize) {
        std::cout << "task not found: " << taskName << std::endl;
        return;
    }

    storage dataStorage;
    dataStorage.saveData(toJson());
    std::cout << "deleted task: " << taskName << std::endl;
}

double action::currentDayTotal() const {
    const std::string date = currentDate();
    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") == date) {
            return log.value("total", 0.0);
        }
    }
    return 0.0;
}

nlohmann::json action::currentDayMarbles() const {
    const std::string date = currentDate();
    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") == date &&
            log.contains("marbles") && log["marbles"].is_array()) {
            return log["marbles"];
        }
    }
    return nlohmann::json::array();
}

void action::recallByDate() {
    if (logs.empty()) {
        std::cout << "no history available" << std::endl;
        return;
    }

    for (const nlohmann::json& log : logs) {
        std::cout << log.value("date", "unknown date") << ": "
                  << log.value("total", 0.0) << " marbles" << std::endl;
    }
}

void action::recallByDate(const std::string& date) {
    const std::string requestedDate =
        !date.empty() && date.front() == '-' ? date.substr(1) : date;
    std::string storedDate;
    if (!convertDate(requestedDate, storedDate)) {
        std::cout << "invalid date: " << date << " (use DD/MM/YY)" << std::endl;
        return;
    }

    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") != storedDate) {
            continue;
        }

        std::cout << requestedDate << ": " << log.value("total", 0.0)
                  << " marbles" << std::endl;
        const nlohmann::json tasks = log.value("tasks", nlohmann::json::array());
        if (tasks.is_array()) {
            std::cout << "tasks:" << std::endl;
            for (const nlohmann::json& task : tasks) {
                std::cout << " " << task.value("name", "unknown task")
                          << ": " << task.value("worth", 0.0)
                          << " marbles" << std::endl;
            }
        }
        return;
    }

    std::cout << "no history for " << requestedDate << std::endl;
}

nlohmann::json action::toJson() const {
    nlohmann::json data{
        {"habits", nlohmann::json::array()},
        {"logs", logs}
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
    logs = data.value("logs", nlohmann::json::array());
    if (!logs.is_array()) {
        logs = nlohmann::json::array();
    }
    if (!data.contains("habits") || !data["habits"].is_array()) {
        return;
    }

    bool migratedLogs = false;
    for (nlohmann::json& log : logs) {
        if (log.contains("marbles") && log["marbles"].is_array()) {
            continue;
        }
        const int marbleTotal = static_cast<int>(
            std::floor(log.value("total", 0.0) + 1e-9));
        log["marbles"] = nlohmann::json::array();
        for (int marble = 0; marble < marbleTotal; ++marble) {
            log["marbles"].push_back(randomMarble());
        }
        migratedLogs = true;
    }

    for (const nlohmann::json& habit : data["habits"]) {
        actionRewards.push_back({
            habit.value("name", ""),
            habit.value("reward", 0.0),
            habit.value("repeatable", false),
            habit.value("repeatWorth", 0.0)
        });
    }

    if (migratedLogs) {
        storage dataStorage;
        dataStorage.saveData(toJson());
    }
}