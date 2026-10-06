#pragma once
#include <string>
#include "apod.hpp"

class ApodClient
{
 
    public:
        Apod getApod();
        std::string imgWrite(Apod apod);
        int setWallpaper(std::string filepath);
};
