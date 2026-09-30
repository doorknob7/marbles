#ifndef MARBLES_H
#define MARBLES_H

#include <vector>
#include "Action.h"

/**
 * @class Marbles
 * @brief Helper utility for marble color generation and historical aggregations.
 */
class Marbles {
public:
    Marbles() = default;

    /**
     * @brief Generates a random RGB color structure for marble visualization.
     * @return MarbleRGB Structure containing randomized r, g, and b values (40 to 215).
     */
    static MarbleRGB randomMarble();

    /**
     * @brief Aggregates all earned marbles across all daily logs into a single vector.
     * @param logs Historical daily log collection.
     * @return std::vector<MarbleRGB> Vector containing all historical marbles.
     */
    std::vector<MarbleRGB> totalMarbles(const std::vector<DailyLog>& logs) const;

    /**
     * @brief Filters marbles earned on today's calendar date.
     * @param logs Historical daily log collection.
     * @return std::vector<MarbleRGB> Vector containing today's earned marbles.
     */
    std::vector<MarbleRGB> currentDayMarbles(const std::vector<DailyLog>& logs) const;

    /**
     * @brief Calculates total marble points earned today.
     * @param logs Historical daily log collection.
     * @return double Total reward points earned today.
     */
    double currentDayTotal(const std::vector<DailyLog>& logs) const;
};

#endif