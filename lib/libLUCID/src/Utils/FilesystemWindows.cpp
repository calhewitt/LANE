///////////////////////////////////////////////////////////////////////////////
/// \file FilesystemWindows.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Cross-platform filesystem handling code - Windows specific
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <vector>
#include <string>
#include <windows.h>
#include <tchar.h>
#include "Utils/Filesystem.hpp"

namespace lane {
namespace utils {

std::vector<std::string> getDirectoryContents(
    const std::string& directory
) noexcept {
    std::vector<std::string> fileList;
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
      
    // Find the first file in the directory.
    hFind = FindFirstFile((TCHAR*)(directory + "/*").c_str(), &ffd);
    
    // Loop over the rest of the files
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string path = ffd.cFileName;
            // Don't add current directory marker and previous directory marker
            if (path != "." && path != "..") {
                fileList.push_back(path);
            }
        } while (FindNextFile(hFind, &ffd) != 0);

        FindClose(hFind);
    }
   
    return fileList;
}

} // utils
} // lane
