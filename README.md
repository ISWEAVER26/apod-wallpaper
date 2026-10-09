# apod_wallpaper

C++ program developed for Linux/GNOME that fetches NASA's Astronomy Picture of the Day (APOD) and sets it as your wallpaper.

## Requirements
- Have $HOME variable set to your home directory
- libcurl pkg
- nlohmann_json pkg
- glib-2.0 + gio-2.0 pkgs

## Installation

Build and install the project using CMake:

```bash
cmake -B build 
cmake --build build

cmake --install build
```

## Files created

- **Configuration:** `~/.config/apod_wallpaper/config.json`.
- **Autostart:** `~/.config/autostart/apod_wallpaper.desktop`.

The program binary is installed directly into your local path at `~/.local/bin/nasa_apod`.
