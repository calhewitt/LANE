///////////////////////////////////////////////////////////////////////////////
/// \file Logger.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief A logger class which manages active logging sinks
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

// TODO: Add positional format string support over the top of the log method

#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "Utils/Logger.hpp"
#include "Utils/LoggerSink.hpp"
#include "Utils/Misc.hpp"

namespace lane {
namespace utils {

std::string severityToString(const SeverityLevel level) noexcept {
    switch (level) {
    case SeverityLevel::Warning:
        return "Warning";
        break;
    case SeverityLevel::Error:
        return "Error";
        break;
    case SeverityLevel::Debug: 
        return "Debug";
        break;
    default:
        return "Unknown";
        break;
    }
}

Logger::Logger() = default;

Logger::~Logger() = default;

Logger::Logger(Logger&& other) {
    std::lock_guard<std::mutex> lock(sinksMutex_);
    std::swap(sinks_, other.sinks_);
}

Logger& Logger::operator=(Logger&& other) {
    if (this != &other) {
        std::lock_guard<std::mutex> lock(sinksMutex_);
        std::swap(sinks_, other.sinks_);
    }
    
    return *this;
}

void Logger::removeAllSinks() noexcept {
    std::lock_guard<std::mutex> lock(sinksMutex_);
    sinks_.clear();
}

void Logger::log(const SeverityLevel level, const std::string& message) {
#ifdef NDEBUG
    if (level == SeverityLevel::Debug) {
        return;
    }
#endif
    std::stringstream finalMessage;
    
    // Go through some hoops to get a timestamp
    auto now = std::chrono::system_clock::now();
    std::stringstream dateTime;
    auto timeT = std::chrono::system_clock::to_time_t(now);
    dateTime << std::ctime(&timeT);
    std::string dateString = dateTime.str().substr(0, dateTime.str().length() - 1);
    
    // Construct the final message to log
    finalMessage << severityToString(level) << " -- " << dateString << " | " << message << std::endl;
    
    std::lock_guard<std::mutex> lock(sinksMutex_);
    
    // Write the message to every registered logging sink
    for (const auto& sink : sinks_) {
        sink->write(finalMessage.str());
    }
}

} // utils
} // lane
