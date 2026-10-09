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
#include <gio/gio.h>
#include <thread>

// Parse config file
nlohmann::json ApodClient::parseConf(){
    std::string filePath = "./config.json";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " + filePath << std::endl;
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
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 300L);

    // Retry curl until connection is made
    CURLcode curlresponse = CURLE_FAILED_INIT;
    std::chrono::milliseconds delay{1000};
    while(curlresponse){
        curlresponse = curl_easy_perform(handle);
        if(curlresponse){
            std::this_thread::sleep_for(delay);
            delay *=2;
        }
    };

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
std::string ApodClient::imgWrite(Apod apod, std::string savemode){

    // build path
    std::string homedir = getpwuid(getuid())->pw_dir;
    std::string path = homedir + "/.local/share/apod-wallpaper/";  
    std::filesystem::create_directories(path);
    std::string filename;
    if (savemode == "replace"){
        filename = "apod_wallpaper.jpeg";
    } else if (savemode == "append"){
        filename = apod.date + ".jpeg";
    }
    std::string filepath = path + filename;

    // check file hasn't been created
    if(!std::filesystem::exists(filepath)){
        // create binary
        std::ofstream fp(filepath, std::ios::binary);
        curl_global_init(CURL_GLOBAL_ALL);
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_URL, apod.hdurl.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, imgWriteback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &fp);
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 300L);

        // Retry curl until connection is made
        CURLcode curlresponse = CURLE_FAILED_INIT;
        std::chrono::milliseconds delay{1000};
        while(curlresponse){
            curlresponse = curl_easy_perform(handle);
            if(curlresponse){
                std::this_thread::sleep_for(delay);
                delay *=2;
            }
        };
        curl_easy_cleanup(handle);
    } 

    return filepath;
}

// Set wallpaper
int ApodClient::setWallpaper(std::string filepath){
    GSettings *gsetting = g_settings_new("org.gnome.desktop.background");

    const char* key = "picture-uri";
    const char* keydark = "picture-uri-dark";

    std::string fileuri = "file://" + filepath;

    gboolean light = g_settings_set_string(gsetting, key, fileuri.c_str());
    gboolean dark = g_settings_set_string(gsetting, keydark, fileuri.c_str());

    g_settings_sync();

    g_object_unref(gsetting);

    return 0;
}