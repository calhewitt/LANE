//////////////////////////////////////////////////////////////////////////////
/// \file PixelTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for the pixel handling code
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <map>
#include "Pixel.hpp"


TEST(PixelTest, EqualityTest) {
    auto pixel1 = lane::Pixel(1, 1, 1);
    auto pixel2 = lane::Pixel(0, 0, 0);
    
    EXPECT_EQ(pixel1 == pixel2, false)
        << "Equality test failed.";
    EXPECT_EQ(pixel1 != pixel2, true)
        << "Inequality test failed.";
}

TEST(PixelTest, CopyTest) {
    auto pixel = lane::Pixel(1, 1, 1);
    auto copiedPixel(pixel);
    
    EXPECT_EQ(copiedPixel == pixel, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(copiedPixel != pixel, false)
        << "Inequality test for copy constructor failed.";
}

TEST(PixelTest, AssignTest) {
    auto pixel = lane::Pixel(1, 1, 1);
    auto assignedPixel = pixel;
    
    EXPECT_EQ(assignedPixel == pixel, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(assignedPixel != pixel, false)
        << "Inequality test for copy constructor failed.";
}

TEST(PixelTest, XTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 },
    };

    auto pixel = lane::Pixel();
    
    for (const auto& test : tests) {
        pixel.setX(test.first);
        unsigned int result = pixel.getX();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(PixelTest, YTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 },
    };

    auto pixel = lane::Pixel();
    
    for (const auto& test : tests) {
        pixel.setY(test.first);
        unsigned int result = pixel.getY();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(PixelTest, CTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 },
    };

    auto pixel = lane::Pixel();
    
    for (const auto& test : tests) {
        pixel.setC(test.first);
        unsigned int result = pixel.getC();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(PixelTest, ETest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 },
    };

    auto pixel = lane::Pixel();
    
    for (const auto& test : tests) {
        pixel.setE(test.first);
        unsigned int result = pixel.getE();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}
