#ifndef MARBLES_H
#define MARBLES_H

#include <nlohmann/json.hpp>

class marbles {
public:
    static nlohmann::json randomMarble();
    
    // Calculates total marbles generated across all historical logs
    nlohmann::json totalMarbles(const nlohmann::json& logs) const;
    
    // Calculates marbles generated for today only
    nlohmann::json currentDayMarbles(const nlohmann::json& logs) const;
    
    // Calculates numerical marble total for today
    double currentDayTotal(const nlohmann::json& logs) const;
};

#endif