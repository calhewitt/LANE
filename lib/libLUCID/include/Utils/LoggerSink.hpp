///////////////////////////////////////////////////////////////////////////////
/// \file LoggerSink.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief A logger sink parent class and basic sinks
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef UTILS_LOGGERSINK_HPP
#define UTILS_LOGGERSINK_HPP

#include <string>
#include <fstream>


namespace utils {

///////////////////////////////////////////////////////////////////////////////
/// \brief A pure virtual base class for logger sinks
class LoggerSink {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    virtual ~LoggerSink() = default;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief The pure virtual member function for writing a logging string
    /// to an output
    /// \param message The text to write to the output sink
    virtual void write(const std::string& message) = 0;
};


///////////////////////////////////////////////////////////////////////////////
/// \brief A sink for console logging
class ConsoleSink final : public LoggerSink {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    ConsoleSink() = default;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~ConsoleSink() = default;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Logs a string to the console output
    /// \param message The string to log
    virtual void write(const std::string& message) {
        std::cout << message;
    }
};


///////////////////////////////////////////////////////////////////////////////
/// \brief A sink for file logging
class FileSink final : public LoggerSink {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor. Creates and stores a handle to a file for logging
    /// \param fileName The name of the file to log to
    FileSink(const std::string& fileName = "log.txt") noexcept {
        logFile_.open(
            fileName,
            std::ios::binary | std::ios::out | std::ios::app
        );
    }
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~FileSink() = default;
    
    FileSink(const FileSink& other) = delete;
    
    FileSink(FileSink&& other) = delete;
    
    FileSink& operator=(const FileSink& other) = delete;
    
    FileSink& operator=(FileSink&& other) = delete;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Logs a string to the file output
    /// \param message The string to log
    virtual void write(const std::string& message) {
        logFile_ << message;
    }

private:
    std::ofstream logFile_;
};

} // utils

#endif // UTILS_LOGGERSINK_HPP
