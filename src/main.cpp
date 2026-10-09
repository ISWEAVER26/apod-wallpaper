#include <iostream>
#include "apod_client.hpp"
#include <chrono>
#include <ctime>

int main(){

    ApodClient client;
    
    Apod apod = client.getApod();
    nlohmann::json configjson = client.parseConf();
    std::string filepath = client.imgWrite(apod, configjson["save_mode"]);
    client.setWallpaper(filepath);
    
    return 0;
};