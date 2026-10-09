#include "config_handler.hpp"
#include <nlohmann/json.hpp>
#include <pwd.h>
#include <unistd.h>
#include <fstream> 
#include <iostream>

nlohmann::json ConfigHandler::parseConfig(){

    // Get path ot config
    std::string homedir = getpwuid(getuid())->pw_dir;
    std::string configpath = homedir + "/.config/apod_wallpaper/config.json";

    // Write from config file to str
    std::ifstream file(configpath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " + configpath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    std::string configtxt;
    while(getline(file, line)){
        configtxt += line;
    };

    file.close(); 
    
    nlohmann::json j = nlohmann::json::parse(configtxt);
    return j;
};
