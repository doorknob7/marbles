#include "Storage.h"
#include "Action.h"
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

std::string Storage::getFilePath() const {
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) {
        return "actions.json";
    }

    std::filesystem::path marblesDir = std::filesystem::path(homeDir) / ".marbles";
    std::error_code ec;
    std::filesystem::create_directories(marblesDir, ec);

    return (marblesDir / "actions.json").string();
}

void Storage::saveData(const Action& actionManager) {
    nlohmann::json root = {
        {"habits", nlohmann::json::array()},
        {"logs", nlohmann::json::array()}
    };

    // Serialize habit definitions
    for (const ActionReward& habit : actionManager.getHabits()) {
        root["habits"].push_back({
            {"name", habit.name},
            {"reward", habit.worth},
            {"repeatable", habit.repeatable},
            {"repeatWorth", habit.repeatWorth}
        });
    }

    // Serialize daily execution logs
    for (const DailyLog& log : actionManager.getLogs()) {
        nlohmann::json logJson = {
            {"date", log.date},
            {"total", log.total},
            {"tasks", nlohmann::json::array()},
            {"marbles", nlohmann::json::array()}
        };

        for (const TaskEntry& task : log.tasks) {
            logJson["tasks"].push_back({
                {"name", task.name},
                {"worth", task.worth}
            });
        }

        for (const MarbleRGB& marble : log.marbles) {
            logJson["marbles"].push_back({
                {"r", marble.r},
                {"g", marble.g},
                {"b", marble.b}
            });
        }

        root["logs"].push_back(logJson);
    }

    std::ofstream outputStream(getFilePath());
    if (outputStream.is_open()) {
        outputStream << root.dump(4) << std::endl;
    } else {
        std::cerr << "[STORAGE ERROR] Unable to open path for writing: " << getFilePath() << std::endl;
    }
}

void Storage::loadData(Action& actionManager) {
    std::ifstream inputStream(getFilePath());
    if (!inputStream.is_open()) {
        return;
    }

    try {
        nlohmann::json root;
        inputStream >> root;

        std::vector<ActionReward> loadedHabits;
        if (root.contains("habits") && root["habits"].is_array()) {
            for (const auto& item : root["habits"]) {
                loadedHabits.push_back(ActionReward{
                    item.value("name", ""),
                    item.value("reward", 0.0),
                    item.value("repeatable", false),
                    item.value("repeatWorth", 0.0)
                });
            }
        }

        std::vector<DailyLog> loadedLogs;
        if (root.contains("logs") && root["logs"].is_array()) {
            for (const auto& logItem : root["logs"]) {
                DailyLog log;
                log.date = logItem.value("date", "");
                log.total = logItem.value("total", 0.0);

                if (logItem.contains("tasks") && logItem["tasks"].is_array()) {
                    for (const auto& taskItem : logItem["tasks"]) {
                        log.tasks.push_back(TaskEntry{
                            taskItem.value("name", ""),
                            taskItem.value("worth", 0.0)
                        });
                    }
                }

                if (logItem.contains("marbles") && logItem["marbles"].is_array()) {
                    for (const auto& marbleItem : logItem["marbles"]) {
                        log.marbles.push_back(MarbleRGB{
                            marbleItem.value("r", 120),
                            marbleItem.value("g", 120),
                            marbleItem.value("b", 120)
                        });
                    }
                }

                loadedLogs.push_back(log);
            }
        }

        actionManager.setHabits(loadedHabits);
        actionManager.setLogs(loadedLogs);

    } catch (const std::exception& e) {
        std::cerr << "[STORAGE ERROR] Parse error (" << e.what() << ")." << std::endl;
    }
}