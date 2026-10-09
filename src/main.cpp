#include <iostream>
#include "apod_client.hpp"
#include "config_handler.hpp"
#include <chrono>
#include <ctime>

int main(){

    ApodClient client;
    ConfigHandler config;
    
    Apod apod = client.getApod();
    nlohmann::json configjson = config.parseConfig();
    std::string filepath = client.imgWrite(apod, configjson["save_mode"]);
    client.setWallpaper(filepath);
    
    return 0;
};