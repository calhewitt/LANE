//////////////////////////////////////////////////////////////////////////////
/// \file FrameTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for the frame handling code
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <string>
#include <map>
#include <array>
#include "Frame.hpp"


TEST(FrameTest, EqualityTest) {
    auto frame1 = lucid::Frame();
    frame1.setTimeStamp(1);
    frame1.setTimeStampSub(1);
    frame1.setPixel(1, 1, 1);
    frame1.setChannelID(1);
    auto frame2 = lucid::Frame();
    frame2.setTimeStamp(0);
    frame2.setTimeStampSub(0);
    frame2.setPixel(0, 0, 0);
    frame2.setChannelID(0);
    
    EXPECT_EQ(frame1 == frame2, false)
        << "Equality test failed.";
    EXPECT_EQ(frame1 != frame2, true)
        << "Inequality test failed.";
}

TEST(FrameTest, CopyTest) {
    auto frame = lucid::Frame();
    frame.setTimeStamp(1);
    frame.setTimeStampSub(1);
    frame.setPixel(1, 1, 1);
    frame.setChannelID(1);
    auto copiedFrame(frame);
    
    EXPECT_EQ(copiedFrame == frame, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(copiedFrame != frame, false)
        << "Inequality test for copy constructor failed.";
}

TEST(FrameTest, AssignTest) {
    auto frame = lucid::Frame();
    frame.setTimeStamp(1);
    frame.setTimeStampSub(1);
    frame.setPixel(1, 1, 1);
    frame.setChannelID(1);
    auto copiedFrame = frame;
    
    EXPECT_EQ(copiedFrame == frame, true)
        << "Equality test for copy constructor failed.";
    EXPECT_EQ(copiedFrame != frame, false)
        << "Inequality test for copy constructor failed.";
}

TEST(FrameTest, ChannelIDTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 }
    };

    auto frame = lucid::Frame();
    
    for (const auto& test : tests) {
        frame.setChannelID(test.first);
        unsigned int result = frame.getChannelID();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(FrameTest, TimeStampTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 }
    };

    auto frame = lucid::Frame();
    
    for (const auto& test : tests) {
        frame.setTimeStamp(test.first);
        unsigned int result = frame.getTimeStamp();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(FrameTest, TimeStampSubTest) {
    std::map<unsigned int, unsigned int> tests = {
        { 1, 1 }
    };

    auto frame = lucid::Frame();
    
    for (const auto& test : tests) {
        frame.setTimeStampSub(test.first);
        unsigned int result = frame.getTimeStampSub();
        EXPECT_EQ(test.second, result)
            << "Input: '" << test.first << "' gives result: '"
            << result << "'. Expected: '" << test.second << "'";
    }
}

TEST(FrameTest, PixelTest) {
    std::map<std::array<unsigned int, 3>, std::array<unsigned int, 3>> tests = {
        { 
            { 1, 1, 1 }, { 1, 1, 1 }
        }
    };

    auto frame = lucid::Frame();
    
    for (const auto& test : tests) {
        frame.setPixel(test.first[0], test.first[1], test.first[2]);
        auto result = frame.getPixel(test.first[0], test.first[1]);
        EXPECT_EQ(test.second[0], result.getX())
            << "Input: '" << test.first[0] << "' gives result: '"
            << result << "'. Expected: '" << test.second[0] << "'";
        EXPECT_EQ(test.second[1], result.getY())
            << "Input: '" << test.first[1] << "' gives result: '"
            << result << "'. Expected: '" << test.second[1] << "'";
        EXPECT_EQ(test.second[2], result.getC())
            << "Input: '" << test.first[2] << "' gives result: '"
            << result << "'. Expected: '" << test.second[2] << "'";
    }
    
    ASSERT_EQ(frame.getPixels().size(), tests.size())
            << "Input: '" << frame.getPixels().size() << "Expected: '" 
            << tests.size() << "'";
}
