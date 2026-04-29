//#include <algorithm>
//#include <cmath>
//#include <memory>
//#include <string>
//#include <vector>
#include <ctime>
#include <fstream>               // Basec
#include <iostream>              // Base
#include <sys/stat.h>            // Base
#include "raylib.h"              // 3D
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"              // Gui
#include <nlohmann/json.hpp>     // JSON

namespace ErrorCodes {
    enum Level { NORMAL = 0, WARN = 1, ERROR = 2, FATAL = 3 };

    std::string levelToString(Level lvl) {
        switch(lvl) {
            case NORMAL: return "NORMAL";
            case WARN:   return "WARN";
            case ERROR:  return "ERROR";
            case FATAL:  return "FATAL";
            default:     return "UNKNOWN";
        }
    }
}

namespace Logger {
    std::ofstream logFile;
    
    void createLogsFolder() { 
        mkdir("logs", 0755);
    }
    
    void log(ErrorCodes::Level lvl, const std::string& message) {
        std::time_t now = std::time(nullptr);
        char timeStr[26];
        std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", std::localtime(&now));
        
        logFile << "[" << timeStr << "] "
                << "[" << ErrorCodes::levelToString(lvl) << "] "
                << message << std::endl;
    }
    
    void initLogFile() {
        logFile.open("logs/log.md");
        std::time_t now = std::time(nullptr);
        char timeStr[26];
    
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        logFile << "<<===== ENGINE Start log =====>>" << std::endl;
        logFile << "[TIME]: " << timeStr << std::endl;
    
        std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", std::localtime(&now));
        logFile << "[" << timeStr << "] "
            << "[" << ErrorCodes::levelToString(ErrorCodes::NORMAL) << "] "
            << "Configuration check..." << std::endl;
    
        logFile << "<<====== ENGINE End log ======>>" << std::endl;
        logFile.close();
    }
    
    bool init(bool createLogs) {
        if (!createLogs) return true;
        
        struct stat st;
        if (stat("logs", &st) != 0) {
            std::cout << std::endl;
            std::cout << "[" << ErrorCodes::levelToString(ErrorCodes::WARN) << "]: logs folder doesn't exist!" << std::endl;
            std::cout << "├─ [CHILD]: Creating logs folder..." << std::endl;
            std::cout << "├─ [CHILD]: Creating log.md..." << std::endl;

            createLogsFolder();

            std::cout << "└─ [CHILD]: Created logs folder, log.md!" << std::endl;
        }
        initLogFile();
        return true;
    }
}

int main() {
    std::ifstream configFile("core/config/Global/global.json");

    if (!configFile.is_open()) {
        std::cout << std::endl;
        std::cerr << ErrorCodes::levelToString(ErrorCodes::FATAL) << ": global.json not found!" << std::endl;
        std::cout << std::endl;

        return ErrorCodes::FATAL;
    }

    auto config = nlohmann::json::parse(configFile);
    bool createLogs = config["createLogs"];
    bool postActive = config["postActive"];

    Logger::init(createLogs);

    if (postActive) {
        std::cout << std::endl;
        std::cout << "[POST] Engine started!" << std::endl;
        std::cout << std::endl;

        InitWindow(800, 600, "Engine");

        Camera3D camera = { 0 };
        camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        while (!WindowShouldClose()) {
            UpdateCamera(&camera, CAMERA_FREE);

            BeginDrawing();
            ClearBackground(GRAY);

            BeginMode3D(camera);
            //DrawCube((Vector3){0, 0, 0}, 1.0f, 1.0f, 1.0f, WHITE);
            DrawGrid(20, 1.5f);

            EndMode3D();

            Rectangle panel = { 600, 10, 190, 250 };
            GuiPanel(panel, "Engine Tools");

            if (GuiButton((Rectangle){ 610, 40, 170, 30 }, "Reset camera")) {
                camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
            }

            GuiSlider((Rectangle){ 610, 80, 170, 20 }, "FOV", NULL, &camera.fovy, 30, 120);
            
            static float zoom = 1.0f;
            GuiSlider((Rectangle){ 610, 110, 170, 20 }, "Zoom", NULL, &zoom, 0.1f, 3.0f);
            
            static bool showGrid = true;
            GuiCheckBox((Rectangle){ 610, 140, 20, 20 }, "Grid", &showGrid);

            GuiLabel((Rectangle){ 10, 570, 300, 25 }, TextFormat("FPS: %i", GetFPS()));

            EndDrawing();
        }

        CloseWindow();
    } else {
        std::cout << std::endl;
        std::cout << "[" <<  ErrorCodes::levelToString(ErrorCodes::FATAL) << "]" << ": Post doesn't have activity parameters!" << std::endl;
        std::cout << std::endl;

        return ErrorCodes::FATAL;
    }
}
