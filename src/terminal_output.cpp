#include "terminal_output.hpp"
#include "apod.hpp"
#include "config_handler.hpp"
#include <iostream>

void TerminalOutput::configDetails(){
    ConfigHandler configHandler;
    nlohmann::json config = configHandler.parseConfig();
    std::cout << "[CONFIG] Save mode: " << config["save_mode"] << std::endl;
}

void TerminalOutput::apodDetails(Apod apod){
    std::cout << "[APOD] Title: " << apod.title << std::endl;
    std::cout <<  "[APOD] " << apod.explanation << std::endl;
}