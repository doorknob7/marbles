#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <nlohmann/json.hpp>

class storage {
private:
    std::string getFilePath() const;

public:
    void saveData(const nlohmann::json& data);
    nlohmann::json loadData();
};

#endif