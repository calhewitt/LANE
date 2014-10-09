///////////////////////////////////////////////////////////////////////////////
/// \file Filesystem.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Cross-platform filesystem handling code
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <vector>
#include <string>
#include "Utils/Filesystem.hpp"

namespace utils {

std::string getFileName(const std::string& path) noexcept {
    std::string fileName = "";
    
    auto size = path.size();
    for (unsigned int i = size - 1; i > 0; --i) {
        if (path[i] == '/' || path[i] == '\\') {
            fileName = path.substr(i + 1, size - 1);
            break;
        }
    }
    
    if (fileName == "") {
        fileName = path;
    }
    
    return fileName;
}

std::string getPath(const std::string& fullPath) noexcept {
    std::string path = "";
    
    auto size = fullPath.size();
    for (unsigned int i = size - 1; i > 0; --i) {
        if (fullPath[i] == '/' || fullPath[i] == '\\') {
            path = fullPath.substr(0, i);
            break;
        }
    }
    
    if (path == "") {
        path = ".";
    }
    
    return path;
}

std::string getExtension(const std::string& path) noexcept {
    std::string extension = "";
    std::string fileName = getFileName(path);
    
    auto size = fileName.size();
    for (unsigned int i = size - 1; i > 0; --i) {
        // Check if it's not the first character to avoid
        // confusion with the current directory symbol
        if (i != 0 && fileName[i] == '.') {
            extension = fileName.substr(i + 1, size - i);
            break;
        }
    }
    
    return extension;
}

} // utils
