// Logger.h
#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <shared_mutex>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();

    void writeLog(const std::string& message);
    std::string readLog();

private:
    std::string logFileName; 
    std::ofstream logFile;
    mutable std::shared_mutex mutex; 
};

#endif // LOGGER_H

