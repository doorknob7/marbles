#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <string>
#include <vector>
#include "Action.h"

/**
 * @class View
 * @brief Handles ANSI terminal rendering, ASCII jar frames, and console output formatting.
 */
class View {
private:
    static constexpr int jarCapacity = 100; ///< Total jar capacity (6 top + 88 middle + 6 bottom)

    /**
     * @brief Draws the ASCII jar frame and positions RGB marble elements.
     * @param marbles Complete array of earned marble structs.
     * @param startOffset Starting index in marbles array.
     * @param count Number of marbles to render inside this frame.
     */
    void drawJarFrame(const std::vector<MarbleRGB>& marbles, int startOffset, int count) const;

public:
    View() = default;

    /**
     * @brief Renders the active jar based on total earned marbles.
     * @param marbles Vector of all-time earned MarbleRGB structs.
     */
    void displayMarbleJar(const std::vector<MarbleRGB>& marbles = {}) const;

    /**
     * @brief Sequentially renders all full jars followed by the active jar.
     * @param marbles Vector of all-time earned MarbleRGB structs.
     */
    void displayAllJars(const std::vector<MarbleRGB>& marbles = {}) const;

    /**
     * @brief Renders home screen layout (Active jar frame + Help reference).
     * @param marbles Vector of all-time earned MarbleRGB structs.
     */
    void showHomeScreen(const std::vector<MarbleRGB>& marbles = {}) const;

    /**
     * @brief Displays command reference list WITHOUT rendering a marble jar.
     */
    void displayHelpMenu() const;

    /**
     * @brief Displays formatted message notice.
     * @param message Text notice string.
     */
    void displayMessage(const std::string& message) const;

    /**
     * @brief Displays list of configured habit definitions.
     * @param habits Collection of ActionReward structs.
     */
    void displayTasks(const std::vector<ActionReward>& habits) const;

    /**
     * @brief Displays history summaries across recorded dates.
     * @param logs Collection of DailyLog structs.
     */
    void displayHistorySummary(const std::vector<DailyLog>& logs) const;

    /**
     * @brief Displays detailed task breakdown for a specific date.
     * @param userDate Formatted date string for header.
     * @param log Pointer to DailyLog struct, or nullptr if unrecorded.
     */
    void displayHistoryForDate(const std::string& userDate, const DailyLog* log) const;
};

#endif