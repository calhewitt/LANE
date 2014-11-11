//////////////////////////////////////////////////////////////////////////////
/// \file BlobTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for the blob data structure
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include "Pixel.hpp"
#include "Blob.hpp"

TEST(BlobTest, EqualityTest) {
    auto blob1 = lane::Blob();
    blob1.addPixelKey(12);
    auto blob2 = lane::Blob();
    blob2.addPixelKey(15);

    EXPECT_EQ(blob1 == blob2, false)
        << "Equality test failed.";
    EXPECT_EQ(blob1 != blob2, true)
        << "Inequality test failed.";
}

TEST(BlobTest, CopyTest) {
    auto blob = lane::Blob();
    blob.addPixelKey(10);
    auto copiedBlob(blob);

    EXPECT_EQ(copiedBlob == blob, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(copiedBlob != blob, false)
        << "Inequality test for copy constructor failed.";
}

TEST(BlobTest, AssignTest) {
    auto blob = lane::Blob();
    blob.addPixelKey(10);
    auto assignedBlob = blob;

    EXPECT_EQ(assignedBlob == blob, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(assignedBlob != blob, false)
        << "Inequality test for copy constructor failed.";
}

TEST(BlobTest, AddPixelTest) {
    auto blob = lane::Blob();
    blob.addPixel(lane::Pixel(1, 1, 1));
    unsigned int result = *(std::begin(blob));
    EXPECT_EQ((unsigned int)(1 * 256 + 1), result)
        << "Input: '1, 1, 1' gives result: '"
        << result << "'. Expected: '257'";
}

TEST(BlobTest, AddPixelKeyTest) {
    auto blob = lane::Blob();
    blob.addPixelKey(30);
    unsigned int result = *(std::begin(blob));
    EXPECT_EQ((unsigned int)30, result)
        << "Input: '30' gives result: '"
        << result << "'. Expected: '30'";
}
