#include "marbles.h"
#include "utils.h"
#include <random>

nlohmann::json marbles::randomMarble() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> color(40, 215);
    return {
        {"r", color(generator)},
        {"g", color(generator)},
        {"b", color(generator)}
    };
}

nlohmann::json marbles::totalMarbles(const nlohmann::json& logs) const {
    nlohmann::json allMarbles = nlohmann::json::array();
    if (!logs.is_array()) return allMarbles;

    for (const nlohmann::json& log : logs) {
        if (log.contains("marbles") && log["marbles"].is_array()) {
            for (const nlohmann::json& marble : log["marbles"]) {
                allMarbles.push_back(marble);
            }
        }
    }
    return allMarbles;
}

nlohmann::json marbles::currentDayMarbles(const nlohmann::json& logs) const {
    const std::string date = utils::currentDate();
    if (!logs.is_array()) return nlohmann::json::array();

    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") == date &&
            log.contains("marbles") && log["marbles"].is_array()) {
            return log["marbles"];
        }
    }
    return nlohmann::json::array();
}

double marbles::currentDayTotal(const nlohmann::json& logs) const {
    const std::string date = utils::currentDate();
    if (!logs.is_array()) return 0.0;

    for (const nlohmann::json& log : logs) {
        if (log.value("date", "") == date) {
            return log.value("total", 0.0);
        }
    }
    return 0.0;
}