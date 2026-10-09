#pragma once
#include <string>
#include "apod.hpp"
#include <nlohmann/json.hpp>

class ApodClient
{
    public:
        Apod getApod();
        std::string imgWrite(Apod apod, std::string save_mode);
        int setWallpaper(std::string filepath);
        nlohmann::json parseConf();
};
