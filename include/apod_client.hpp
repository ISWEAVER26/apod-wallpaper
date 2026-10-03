#pragma once
#include <string>
#include "apod.hpp"

class ApodClient
{
 
    public:
        Apod getApod();
        int imgWrite(Apod apod);
};
