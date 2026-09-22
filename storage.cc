#include "storage.h"
#include <fstream>

namespace {
const char* dataFile = "include/actions.json";
}

void storage::saveData(const nlohmann::json& data) {
    nlohmann::json savedData = nlohmann::json::object();
    std::ifstream input(dataFile);
    if (input) {
        try {
            input >> savedData;
        } catch (const nlohmann::json::parse_error&) {
            savedData = nlohmann::json::object();
        }
    }

    savedData["habits"] = data.value("habits", nlohmann::json::array());
    if (!savedData.contains("logs") || !savedData["logs"].is_array()) {
        savedData["logs"] = nlohmann::json::array();
    }

    std::ofstream output(dataFile);
    if (output) {
        output << savedData.dump(4) << std::endl;
    }
}

nlohmann::json storage::loadData() {
    std::ifstream input(dataFile);
    if (!input) {
        return nlohmann::json{{"habits", nlohmann::json::array()},
                              {"logs", nlohmann::json::array()}};
    }

    try {
        nlohmann::json data;
        input >> data;
        return data;
    } catch (const nlohmann::json::parse_error&) {
        std::cout << "unable to read saved data" << std::endl;
        return nlohmann::json{{"habits", nlohmann::json::array()},
                              {"logs", nlohmann::json::array()}};
    }
}