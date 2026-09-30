#include "Action.h"
#include "Utils.h"
#include "Marbles.h"
#include <algorithm>
#include <cmath>

bool Action::addHabit(const std::string& name, double worth, bool repeatable, double repeatWorth) {
    // Domain invariant check: Reject empty names or negative marble worth values
    if (name.empty() || worth < 0.0 || repeatWorth < 0.0) {
        return false;
    }

    // Verify habit name is unique
    auto existing = std::find_if(habits.begin(), habits.end(),
        [&name](const ActionReward& h) { return h.name == name; });

    if (existing != habits.end()) {
        return false;
    }

    habits.push_back(ActionReward{name, worth, repeatable, repeatWorth});
    return true;
}

bool Action::deleteHabit(const std::string& name) {
    const std::size_t originalSize = habits.size();
    habits.erase(
        std::remove_if(habits.begin(), habits.end(),
                       [&name](const ActionReward& h) { return h.name == name; }),
        habits.end());

    return habits.size() < originalSize;
}

Action::LogStatus Action::logTask(const std::string& taskName, int& newMarblesAdded) {
    newMarblesAdded = 0;

    // Search for habit configuration
    const auto habit = std::find_if(habits.begin(), habits.end(),
        [&taskName](const ActionReward& h) { return h.name == taskName; });

    if (habit == habits.end()) {
        return LogStatus::TASK_NOT_FOUND;
    }

    const std::string todayDate = utils::currentDate();
    DailyLog* todayLog = nullptr;

    // Locate daily log record for today
    for (DailyLog& log : logs) {
        if (log.date == todayDate) {
            todayLog = &log;
            break;
        }
    }

    // Create a new daily log if unrecorded
    if (todayLog == nullptr) {
        logs.push_back(DailyLog{todayDate, 0.0, {}, {}});
        todayLog = &logs.back();
    }

    // Count completions of this habit today
    std::size_t loggedCount = 0;
    for (const TaskEntry& entry : todayLog->tasks) {
        if (entry.name == taskName) {
            ++loggedCount;
        }
    }

    // Block non-repeatable habits from multiple daily entries
    if (!habit->repeatable && loggedCount > 0) {
        return LogStatus::ALREADY_LOGGED_TODAY;
    }

    // Calculate marble reward value using clean control flow
    double worth = habit->worth;
    if (loggedCount > 0 && habit->repeatWorth > 0.0) {
        worth = habit->repeatWorth;
    }

    const double previousTotal = todayLog->total;
    todayLog->tasks.push_back(TaskEntry{taskName, worth});
    todayLog->total += worth;

    // Compute marble increments
    const int previousMarbles = static_cast<int>(std::floor(previousTotal + 1e-9));
    const int currentMarbles = static_cast<int>(std::floor(todayLog->total + 1e-9));
    newMarblesAdded = std::max(0, currentMarbles - previousMarbles);

    // Generate random colors for new marbles
    for (int i = 0; i < newMarblesAdded; ++i) {
        todayLog->marbles.push_back(Marbles::randomMarble());
    }

    return LogStatus::SUCCESS;
}

const DailyLog* Action::getLogForDate(const std::string& isoDate) const {
    for (const DailyLog& log : logs) {
        if (log.date == isoDate) {
            return &log;
        }
    }
    return nullptr;
}