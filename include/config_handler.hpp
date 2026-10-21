#pragma once
#include <nlohmann/json.hpp>

class ConfigHandler{

    public:
        nlohmann::json parseConfig();
};