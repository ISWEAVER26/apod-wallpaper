#include <iostream>
#include "apod_client.hpp"
#include <chrono>
#include <ctime>

int main(){

    ApodClient client;

    Apod apod = client.getApod();
    client.imgWrite(apod);

    return 0;
};