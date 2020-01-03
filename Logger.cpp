#include "Logger.h"
#include <algorithm>

asf::Logger::Logger() {
    fileName = "test_file.txt";
    if ( logFile == nullptr ) {
        logFile = fopen(fileName.c_str(), "a+");
    }
}

asf::Logger::Logger( std::string name ) {
    fileName = name;
    if ( logFile == nullptr ) {
        logFile = fopen(fileName.c_str(), "a+" );
    }
}

asf::Logger::~Logger() {
    if ( logFile != nullptr ) {
        fclose(logFile);
        logFile = nullptr;
    }
}

void asf::Logger::log(std::string message) {
    if ( logFile == nullptr ) {
        logFile = fopen(fileName.c_str(), "a+");
    }
    if ( logFile != nullptr ) {
        auto time = std::chrono::system_clock::now();
        std::time_t logTime = std::chrono::system_clock::to_time_t(time);
        std::string timeString = std::ctime(&logTime);
        timeString.erase(std::remove(timeString.begin(), timeString.end(), '\n'), timeString.end());

        fwprintf(logFile, L"[%s]: %s\n", timeString.c_str(), message.c_str());

        if ( ferror(logFile) ) {
            std::cerr << "There was an error writing to the file" << std::endl;
            if ( errno ) {
                std::cerr << "Error was: " << std::strerror(errno) << std::endl;
            }
        }

        fclose(logFile);
        logFile = nullptr;
    }
}