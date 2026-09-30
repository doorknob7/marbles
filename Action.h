#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <vector>

/**
 * @struct ActionReward
 * @brief Represents a user-defined habit and its associated marble rewards.
 */
struct ActionReward {
    std::string name;
    double worth = 0.0;
    bool repeatable = false;
    double repeatWorth = 0.0;
};

/**
 * @struct TaskEntry
 * @brief Represents an individual habit completion recorded on a specific day.
 */
struct TaskEntry {
    std::string name;
    double worth = 0.0;
};

/**
 * @struct MarbleRGB
 * @brief Represents 24-bit RGB color channels for a single marble.
 */
struct MarbleRGB {
    int r = 120;
    int g = 120;
    int b = 120;
};

/**
 * @struct DailyLog
 * @brief Stores habit execution entries and earned marbles for a single calendar day.
 */
struct DailyLog {
    std::string date;                 ///< Date formatted as YYYY-MM-DD
    double total = 0.0;               ///< Accumulated marble points for the day
    std::vector<TaskEntry> tasks;     ///< List of habits completed on this date
    std::vector<MarbleRGB> marbles;   ///< Generated RGB marble visual objects
};

/**
 * @class Action
 * @brief Domain manager for habit configurations and daily completion logs.
 */
class Action {
private:
    std::vector<ActionReward> habits; ///< In-memory collection of configured habits
    std::vector<DailyLog> logs;       ///< In-memory collection of daily execution logs

public:
    /**
     * @enum LogStatus
     * @brief Return codes representing the outcome of a habit logging attempt.
     */
    enum class LogStatus {
        SUCCESS,
        TASK_NOT_FOUND,
        ALREADY_LOGGED_TODAY
    };

    Action() = default;

    /**
     * @brief Adds a new habit configuration to memory.
     * @param name Unique string identifier for the habit.
     * @param worth Base marble reward value.
     * @param repeatable Flag indicating if habit can be logged multiple times per day.
     * @param repeatWorth Secondary marble reward value for repeat completions.
     * @return true if habit was registered successfully, false if name is invalid or duplicated.
     */
    bool addHabit(const std::string& name, double worth, bool repeatable, double repeatWorth = 0.0);

    /**
     * @brief Deletes an existing habit configuration by name.
     * @param name Target habit identifier to erase.
     * @return true if habit was found and erased, false otherwise.
     */
    bool deleteHabit(const std::string& name);

    /**
     * @brief Returns a const reference to registered habits.
     * @return const std::vector<ActionReward>& List of habit definitions.
     */
    const std::vector<ActionReward>& getHabits() const { return habits; }

    /**
     * @brief Logs habit execution for today and computes newly earned marbles.
     * @param taskName Target habit identifier.
     * @param newMarblesAdded Output reference populated with the count of newly generated marbles.
     * @return LogStatus Result status code.
     */
    LogStatus logTask(const std::string& taskName, int& newMarblesAdded);

    /**
     * @brief Returns a const reference to historical daily logs.
     * @return const std::vector<DailyLog>& List of daily log records.
     */
    const std::vector<DailyLog>& getLogs() const { return logs; }

    /**
     * @brief Finds a daily log record matching a specific ISO date.
     * @param isoDate Target date string formatted as YYYY-MM-DD.
     * @return const DailyLog* Pointer to matching record, or nullptr if unrecorded.
     */
    const DailyLog* getLogForDate(const std::string& isoDate) const;

    /**
     * @brief Overwrites internal habits vector (used during data loading).
     * @param loadedHabits Deserialized habit definitions.
     */
    void setHabits(const std::vector<ActionReward>& loadedHabits) { habits = loadedHabits; }

    /**
     * @brief Overwrites internal logs vector (used during data loading).
     * @param loadedLogs Deserialized daily log records.
     */
    void setLogs(const std::vector<DailyLog>& loadedLogs) { logs = loadedLogs; }
};

#endif