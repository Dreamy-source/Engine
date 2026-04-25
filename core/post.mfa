//#include <algorithm>
//#include <cmath>
//#include <memory>
//#include <string>
//#include <vector>
//#include <ctime>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <nlohmann/json.hpp>

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

        return ErrorCodes::NORMAL;
    } else {
        std::cout << std::endl;
        std::cout << "[" <<  ErrorCodes::levelToString(ErrorCodes::FATAL) << "]" << ": Post doesn't have activity parameters!" << std::endl;
        std::cout << std::endl;

        return ErrorCodes::FATAL;
    }
}
