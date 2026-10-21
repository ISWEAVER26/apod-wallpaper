#include <iostream>
#include "apod_client.hpp"
#include "config_handler.hpp"
#include <chrono>
#include <ctime>
#include "terminal_output.hpp"

int main(){

    ApodClient client;
    ConfigHandler config;
    TerminalOutput terminal;
    
    // Output config detail
    terminal.configDetails();

    // Open APOD connection
    Apod apod = client.getApod();

    // Write APOD to disk
    nlohmann::json configjson = config.parseConfig();
    std::string filepath = client.imgWrite(apod, configjson["save_mode"]);

    // Set wallp
    client.setWallpaper(filepath);

    // Output detail
    terminal.apodDetails(apod);
    return 0;
};