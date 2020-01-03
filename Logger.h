#pragma once
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <chrono>
#include <ctime>

namespace asf {
    class Logger {
        public:
        Logger();
        Logger(std::string name);
        ~Logger();

        std::string fileName;
        FILE * logFile = nullptr;

        void log( std::string message );
    };
}