#ifndef STORAGE_H
#define STORAGE_H

#include <string>

// Forward declaration avoids importing full Action definition into this header
class Action;

class Storage {
private:
    std::string getFilePath() const;

public:
    Storage() = default;
    void saveData(const Action& actionManager);
    void loadData(Action& actionManager);
};

#endif