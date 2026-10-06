#include <iostream>
#include "apod_client.hpp"
#include <chrono>
#include <ctime>

int main(){

    ApodClient client;
    
    Apod apod = client.getApod();
    std::string filepath = client.imgWrite(apod);
    client.setWallpaper(filepath);
    return 0;
};