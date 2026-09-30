#include "Marbles.h"
#include "Utils.h"
#include <random>

MarbleRGB Marbles::randomMarble() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> color(40, 215);
    return MarbleRGB{color(generator), color(generator), color(generator)};
}

std::vector<MarbleRGB> Marbles::totalMarbles(const std::vector<DailyLog>& logs) const {
    std::vector<MarbleRGB> allMarbles;
    for (const DailyLog& log : logs) {
        allMarbles.insert(allMarbles.end(), log.marbles.begin(), log.marbles.end());
    }
    return allMarbles;
}

std::vector<MarbleRGB> Marbles::currentDayMarbles(const std::vector<DailyLog>& logs) const {
    const std::string date = utils::currentDate();
    for (const DailyLog& log : logs) {
        if (log.date == date) {
            return log.marbles;
        }
    }
    return {};
}

double Marbles::currentDayTotal(const std::vector<DailyLog>& logs) const {
    const std::string date = utils::currentDate();
    for (const DailyLog& log : logs) {
        if (log.date == date) {
            return log.total;
        }
    }
    return 0.0;
}