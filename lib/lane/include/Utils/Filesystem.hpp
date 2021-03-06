///////////////////////////////////////////////////////////////////////////////
/// \file Filesystem.hpp
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

#ifndef LANE_UTILS_FILESYSTEM_HPP
#define LANE_UTILS_FILESYSTEM_HPP

#include <vector>
#include <string>

namespace lane {
namespace utils {

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets a list of file-paths for all files in a given directory.
/// Returns an empty vector if the directory is empty. Searches
/// non-recursively, and includes directories in the result.
/// \param directory The directory to search in for files.
/// \return A vector of file-paths.
std::vector<std::string> getDirectoryContents(
    const std::string& directory
) noexcept;

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the file name string of a path. Returns an empty string if 
/// there is no file name.
/// \param path The path string you want the path from.
/// \return The file name string.
std::string getFileName(const std::string& path) noexcept;

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the directory string of a filename/path. Returns the current
/// directory marker if the path contains only a file name.
/// \param fullPath The full path string that you want the path string from.
/// \return The extension string.
std::string getPath(const std::string& fullPath) noexcept;

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the extension string of a filename/path. Returns an empty
/// string if there is no extension.
/// \param path The path string that you want the extension from.
/// \return The extension string.
std::string getExtension(const std::string& path) noexcept;

///////////////////////////////////////////////////////////////////////////////
/// \brief Removes the extension string of a filename/path. Returns an empty
/// string if there is no extension.
/// \param path The path string that you want to remove the extension from.
/// \return The path/filename without the extension.
std::string removeExtension(const std::string& path) noexcept;


///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the file paths with the give nextension in a given directory
/// \param extension The extension to check for
/// \param directory The directory to search for the files
/// \return A list of path names for files with the given extension in the
/// directory
std::vector<std::string> getFilesWithExtension(
    const std::string& extension,
    const std::string& directory = "./"
) noexcept;


///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the file paths with the give nextension in a list of file
/// path strings
/// \param extension The extension to check for
/// \param files The list of file path strings to check through
/// \return A list of path names for files with the given extension in the
/// directory
std::vector<std::string> getFilesWithExtension(
    const std::string& extension,
    const std::vector<std::string>& files
) noexcept;

} // utils
} // lane

#endif // LANE_UTILS_FILESYSTEM_HPP
