//////////////////////////////////////////////////////////////////////////////
/// \file LUCIDFileTest.cpp
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
#include <string>
#include <vector>
#include "LUCID/LUCIDFile.hpp"

const unsigned int numberOfFiles = 2;
const std::string dataPath = "../../../testdata/";
const std::string dataFiles[numberOfFiles] = {
    "TestFile1.ldat",
    "TestFile2.ldat"
};

class LUCIDFileTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {
        ASSERT_NO_THROW({
            for (unsigned int i = 0; i < numberOfFiles; ++i) {
                files_.push_back(lucid::LUCIDFile());
                files_[i].read(dataPath + dataFiles[i]);
            }
        }) << "The LUCIDFile reading test threw an exception.";
    }

    virtual void SetUp() {
    }
    virtual void TearDown() {
    }

    static std::vector<lucid::LUCIDFile> files_;
};

std::vector<lucid::LUCIDFile> LUCIDFileTest::files_ = std::vector<lucid::LUCIDFile>();

TEST_F(LUCIDFileTest, EqualityTest) {
    EXPECT_EQ(files_[0] == files_[1], false)
        << "Equality test failed.";
    EXPECT_EQ(files_[0] != files_[1], true)
        << "Inequality test failed.";
}

TEST_F(LUCIDFileTest, CopyTest) {
    auto copiedFile(files_[0]);
    EXPECT_EQ(copiedFile == files_[0], true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(copiedFile != files_[0], false)
        << "Inequality test for copy constructor failed.";
}

TEST_F(LUCIDFileTest, AssignTest) {
    auto assignedFile = files_[0];
    EXPECT_EQ(assignedFile == files_[0], true)
        << "Equality test for assignment operator failed.";
    EXPECT_EQ(assignedFile != files_[0], false)
        << "Inequality test for assignment operator failed.";
}
