///////////////////////////////////////////////////////////////////////////////
/// \file FilesystemLinux.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Cross-platform filesystem handling code - Linux specific
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "Utils/Filesystem.hpp"

namespace utils {

// Supports posix systems
std::vector<std::string> getDirectoryContents(
    const std::string& directory
) noexcept {
    std::vector<std::string> fileList;
    DIR *d;
    struct dirent *dir;
    
    d = opendir(directory.c_str());
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            std::string path = dir->d_name;
            // Don't add current directory marker and previous directory marker
            if (path != "." && path != "..") {
                    fileList.push_back(path);
            }
        }

        closedir(d);
    }
    
    return fileList;
}

} // utils
