///////////////////////////////////////////////////////////////////////////////
/// \file LoggerSink.cpp
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

#include <iostream>
#include <string>
#include <fstream>
#include "Utils/LoggerSink.hpp"


namespace utils {

// LoggerSink implementations
LoggerSink::~LoggerSink() = default;


// ConsoleSink implementations
ConsoleSink::ConsoleSink() = default;

ConsoleSink::~ConsoleSink() = default;

void ConsoleSink::write(const std::string& message) {
    std::cout << message;
}


// FileSink implementations
FileSink::FileSink(const std::string& fileName) noexcept {
    logFile_.open(
        fileName,
        std::ios::binary | std::ios::out | std::ios::app
    );
}

FileSink::~FileSink() = default;

void FileSink::write(const std::string& message) {
    logFile_ << message;
}

} // utils
