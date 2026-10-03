#include "apod_client.hpp"
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <format>
#include <fstream> 
#include <pwd.h>
#include <unistd.h>

// Write from buffer to file
size_t imgWriteback(char* buffp, size_t datasize, size_t itemct, void* userp) {
    std::ofstream* response = static_cast<std::ofstream*>(userp);

    size_t chunksize = datasize*itemct;

    response->write(buffp, chunksize);

    return chunksize;
};

// Write from buffer to apod struct
size_t clientWriteback(char* buffp, size_t datasize, size_t itemct, void* userp){
    std::string* response = static_cast<std::string*>(userp);
    
    for(int i = 0; i < (datasize*itemct); i++){
        *response += buffp[i];
    };

    return datasize*itemct;
};

// Get sysdate
std::tm* systemDate(){
    const auto now = std::chrono::system_clock::now();
    time_t t_t = std::chrono::system_clock::to_time_t(now);
    std::tm* date = std::localtime(&t_t);
    return date;
};

// Call Nasa endpoint
Apod ApodClient::getApod(){
    std::string response;
    std::string year = std::to_string(systemDate()->tm_year + 1900);
    std::string month = std::to_string(systemDate()->tm_mon + 1);
    std::string day = std::format("{:02}", systemDate()->tm_mday);
    std::string url = "https://science.nasa.gov/wp-json/wp/v2/apod-basic/" 
                      + year.substr(2,2) + month + day;    
    
    // Handle libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, clientWriteback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    // Parse libcurl response
    nlohmann::json j = nlohmann::json::parse(response);
    
    // return Apod 
    Apod apod{
        .date = j["date"],
        .hdurl = j["hdurl"],
        .media_type = j["media_type"]
    };
    
    return apod;
};

// Write from apod struct to file
int ApodClient::imgWrite(Apod apod){
    std::string filename = apod.date + ".jpeg";
    std::ofstream fp(filename, std::ios::binary);
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, apod.hdurl.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, imgWriteback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &fp);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    return 0;
}