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

#ifndef LANE_UTILS_LOGGERSINK_HPP
#define LANE_UTILS_LOGGERSINK_HPP

#include <string>
#include <fstream>

namespace lane {
namespace utils {

///////////////////////////////////////////////////////////////////////////////
/// \brief A pure virtual base class for logger sinks
class LoggerSink {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    virtual ~LoggerSink();
    
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
    ConsoleSink();
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~ConsoleSink();
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Logs a string to the console output
    /// \param message The string to log
    virtual void write(const std::string& message);
};


///////////////////////////////////////////////////////////////////////////////
/// \brief A sink for file logging
class FileSink final : public LoggerSink {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor. Creates and stores a handle to a file for logging
    /// \param fileName The name of the file to log to
    FileSink(const std::string& fileName = "log.txt") noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~FileSink();
    
    FileSink(const FileSink& other) = delete;
    
    FileSink(FileSink&& other) = delete;
    
    FileSink& operator=(const FileSink& other) = delete;
    
    FileSink& operator=(FileSink&& other) = delete;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Logs a string to the file output
    /// \param message The string to log
    virtual void write(const std::string& message);

private:
    std::ofstream logFile_;
};

} // utils
} // lane

#endif // LANE_UTILS_LOGGERSINK_HPP
