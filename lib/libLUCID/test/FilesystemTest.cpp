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
#include <iostream>
#include "Utils/Filesystem.hpp"

const unsigned int numberOfFiles = 2;
const std::string dataPath = "../../../testdata/";

TEST(Filesystem, GetFilesTest) {
    auto fileList = utils::getDirectoryContents(dataPath);
    EXPECT_EQ(numberOfFiles, fileList.size());
}

TEST(Filesystem, GetExtensionTest) {
    auto fileList = utils::getDirectoryContents(dataPath);
    auto extension1 = utils::getExtension(fileList[0]);
    auto extension2 = utils::getExtension(fileList[1]);
    EXPECT_STREQ("", extension1.c_str());
    EXPECT_STREQ("", extension2.c_str());
}

TEST(Filesystem, GetFileNameTest) {
    auto fileList = utils::getDirectoryContents(dataPath);
    auto fileName1 = utils::getFileName(fileList[0]);
    auto fileName2 = utils::getFileName(fileList[1]);
    EXPECT_STREQ("TestFile1", fileName1.c_str());
    EXPECT_STREQ("TestFile2", fileName2.c_str());
}
