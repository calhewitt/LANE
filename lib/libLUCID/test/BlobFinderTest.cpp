//////////////////////////////////////////////////////////////////////////////
/// \file BlobFinderTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for finding blobs of pixels
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <string>
#include "Frame.hpp"
#include "BlobFinder.hpp"

namespace {
    
void clearFrame(lucid::Frame& frame, unsigned int c = 0) noexcept {
    for (unsigned int i = 0; i < 256; ++i) {
        for (unsigned int j = 0; j < 256; ++j) {
            frame.setPixel(i, j, c);
        }
    }
}
    
}

TEST(BlobFinderTest, FindBlobsTest) {
    lucid::Frame testFrame;
    clearFrame(testFrame, 1);
    
    // Test 1
    unsigned int result = lucid::findBlobs(testFrame).size();
    EXPECT_EQ(1, result) 
        << "Input: 'all ones' gives result: '"
        << result << "'. Expected: '1'";

    // Test 2
    clearFrame(testFrame, 0);
    for (unsigned int i = 0; i < 256; ++i) {
        testFrame.setPixel(3, i, 1);
        testFrame.setPixel(9, i, 1);
    }
    result = lucid::findBlobs(testFrame).size();
    EXPECT_EQ(2, result) 
        << "Input: 'two lines of values' gives result: '"
        << result << "'. Expected: '2'";
        
    // Test 3
    for (unsigned int i = 0; i < 256; ++i) {
        testFrame.setPixel(i, 3, 1);
    }
    result = lucid::findBlobs(testFrame).size();
    EXPECT_EQ(1, result) 
        << "Input: 'two lines of values with one line crossing' gives result: '"
        << result << "'. Expected: '1'";
}
