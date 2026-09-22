#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class storage {

    private:
        

    public:
        storage(){};
        void saveData(const nlohmann::json& data);
        nlohmann::json loadData();


};

#endif