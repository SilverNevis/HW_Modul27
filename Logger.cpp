#include "Logger.h"
#include <iostream>
#include <fstream>

Logger::Logger(const std::string& filename) : logFileName(filename), logFile(filename, std::ios::app) {
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::writeLog(const std::string& message) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
    else {
        std::cerr << "Error: Log file is not open." << std::endl;
    }
}

std::string Logger::readLog() {
    std::string line;
    std::ifstream readFile(logFileName); 
    if (!readFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
        return "";
    }

    std::getline(readFile, line);
    return line;
}







