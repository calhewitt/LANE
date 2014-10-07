//////////////////////////////////////////////////////////////////////////////
/// \file FileReadTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for reading LUCID data files.
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "LUCID/LUCIDFile.hpp"

TEST(FileReadTest, File) {
    const unsigned int numberOfFiles = 2;
    const std::string dataPath = "../../../testdata/";
    const std::string dataFiles[numberOfFiles] = {
        "TestFile1",
        "TestFile2"
    };

    ASSERT_NO_THROW({
        for (unsigned int i = 0; i < numberOfFiles; ++i) {
            auto file = lucid::LUCIDFile(dataPath + dataFiles[i]);
            //std::cout << file << "\n";
        }
    });
}

