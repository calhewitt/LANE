///////////////////////////////////////////////////////////////////////////////
/// \file Logger.hpp
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

#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

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
#include "LoggerSink.hpp"
#include "Misc.hpp"


namespace utils {

///////////////////////////////////////////////////////////////////////////////
/// \brief The level of severity of the log message
enum class SeverityLevel {
    Warning,
    Error,
    Debug,
};


///////////////////////////////////////////////////////////////////////////////
/// \brief The level of severity of the log message
/// \param level A logging severity level
/// \returns A text string of the input SeverityLevel
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


///////////////////////////////////////////////////////////////////////////////
/// \brief Manages logging sinks
class Logger final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Logger() = default;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Logger() = default;
    
    Logger(const Logger& other) = delete;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Logger(Logger&& other) {
        std::lock_guard<std::mutex> lock(sinksMutex_);
        std::swap(sinks_, other.sinks_);
    }
    
    Logger& operator=(const Logger& other) = delete;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Logger& operator=(Logger&& other) {
        if (this != &other) {
            std::lock_guard<std::mutex> lock(sinksMutex_);
            std::swap(sinks_, other.sinks_);
        }
        
        return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructs and adds a logging sink to the logging system
    /// \param args The arguments to pass to the sink constructor
    template <class SinkType, typename... Args>
    void addSink(Args&&... args) {
        std::lock_guard<std::mutex> lock(sinksMutex_);
        sinks_.push_back(make_unique<SinkType>(std::forward<Args>(args)...));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Removes and destructs all added sinks from the logging system
    void removeAllSinks() noexcept {
        std::lock_guard<std::mutex> lock(sinksMutex_);
        sinks_.clear();
    }
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds a timestamp, formats the log message and hands that over to
    /// the added sinks in order to write out to the various outputs.
    /// Doesn't do anything if the Debug severity level is used, and the
    /// NDEBUG is defined.
    /// \param level The severity level of the log message
    /// \param message The text to use in the log message
    void log(const SeverityLevel level, const std::string& message) {
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

private:
    std::vector<std::unique_ptr<LoggerSink>> sinks_;
    std::mutex sinksMutex_;
};

} // utils

#endif // UTILS_LOGGER_HPP
