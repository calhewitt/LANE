//////////////////////////////////////////////////////////////////////////////
/// \file FilesystemTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for the cross platform filesystem handling code
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <string>
#include <map>
#include <iostream>
#include "Utils/Filesystem.hpp"

const unsigned int numberOfFiles = 2;
const std::string dataPath = "../../../testdata/";

TEST(FilesystemTest, GetFilesTest) {
    auto fileList = utils::getDirectoryContents(dataPath);
    EXPECT_EQ(numberOfFiles, fileList.size());
}

TEST(FilesystemTest, GetExtensionTest) {
    std::map<std::string, std::string> tests = {
        { "test", "" },
        { "test.txt", "txt" },
        { "./test.txt", "txt" },
        { "../test.txt", "txt" },
        { "/dir/test.txt", "txt" },
        { "/dir/test", "" },
        { "/dir/test.txt", "txt" },
        { "./dir/test.txt", "txt" },
        { "/dir/../test.txt", "txt" }
    };
    
    for (const auto& test : tests) {
        std::string result = utils::getExtension(test.first);
        EXPECT_STREQ(test.second.c_str(), result.c_str())
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(FilesystemTest, RemoveExtensionTest) {
    std::map<std::string, std::string> tests = {
        { "test", "test" },
        { "test.txt", "test" },
        { "./test.txt", "./test" },
        { "../test.txt", "../test" },
        { "/dir/test.txt", "/dir/test" },
        { "/dir/test", "/dir/test" },
        { "/dir/test.txt", "/dir/test" },
        { "./dir/test.txt", "./dir/test" },
        { "/dir/../test.txt", "/dir/../test" }
    };
    
    for (const auto& test : tests) {
        std::string result = utils::removeExtension(test.first);
        EXPECT_STREQ(test.second.c_str(), result.c_str())
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(FilesystemTest, GetFileNameTest) {
    std::map<std::string, std::string> tests = {
        { "test", "test" },
        { "test.txt", "test.txt" },
        { "./test.txt", "test.txt" },
        { "../test.txt", "test.txt" },
        { "/dir/test.txt", "test.txt" },
        { "/dir/test", "test" },
        { "/dir/test.txt", "test.txt" },
        { "./dir/test.txt", "test.txt" },
        { "/dir/../test.txt", "test.txt" }
    };
    
    for (const auto& test : tests) {
        std::string result = utils::getFileName(test.first);
        EXPECT_STREQ(test.second.c_str(), result.c_str())
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}
