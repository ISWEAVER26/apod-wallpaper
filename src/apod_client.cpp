#include "apod_client.hpp"
#include <curl/curl.h>
#include <string>
#include <iostream>

size_t clientWriteback(char* buffp, size_t datasize, size_t itemct, void* userp){
    std::string* response = static_cast<std::string*>(userp);
    
    for(int i = 0; i < (datasize*itemct); i++){
        *response += buffp[i];
    };

    return datasize*itemct;
};

void ApodClient::getApod(){

    std::string response;

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, "https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, clientWriteback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(handle);
    std::cout << response;
    curl_easy_cleanup(handle);

    // return Apod 

};