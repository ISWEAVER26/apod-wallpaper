cmake_minimum_required(VERSION 3.20)

project(apod_wallpaper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET
    glib-2.0
    gio-2.0
)

add_executable(nasa_apod
    src/main.cpp
    src/apod_client.cpp
)

target_include_directories(nasa_apod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(nasa_apod PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    PkgConfig::GLIB
)