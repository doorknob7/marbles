#include "storage.h"
#include <fstream>
#include <cstdlib>
#include <filesystem>

std::string storage::getFilePath() const {
    const char* home = std::getenv("HOME");
    if (!home) return "actions.json";
    
    std::filesystem::path dir = std::filesystem::path(home) / ".marbles";
    std::filesystem::create_directories(dir);
    return (dir / "actions.json").string();
}

void storage::saveData(const nlohmann::json& data) {
    std::ofstream output(getFilePath());
    if (output) {
        output << data.dump(4) << std::endl;
    }
}

nlohmann::json storage::loadData() {
    std::ifstream input(getFilePath());
    if (!input) {
        return nlohmann::json{{"habits", nlohmann::json::array()},
                              {"logs", nlohmann::json::array()}};
    }

    try {
        nlohmann::json data;
        input >> data;
        return data;
    } catch (const nlohmann::json::parse_error&) {
        return nlohmann::json{{"habits", nlohmann::json::array()},
                              {"logs", nlohmann::json::array()}};
    }
}