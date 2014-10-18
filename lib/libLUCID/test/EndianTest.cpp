//////////////////////////////////////////////////////////////////////////////
/// \file EndianTest.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Test for the endian handling code
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include "gtest/gtest.h"
#include <cstdint>
#include <map>
#include "Utils/Endian.hpp"


TEST(EndianTest, EndianSwappingU16Test) {
    std::map<std::uint16_t, std::uint16_t> tests = {
        { 0x0102, 0x0201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}

TEST(EndianTest, EndianSwappingU32Test) {
    std::map<std::uint32_t, std::uint32_t> tests = {
        { 0x01020304, 0x04030201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}

TEST(EndianTest, EndianSwappingU64Test) {
    std::map<std::uint64_t, std::uint64_t> tests = {
        { 0x0102030405060708, 0x0807060504030201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}


TEST(EndianTest, EndianSwapping16Test) {
    std::map<std::int16_t, std::int16_t> tests = {
        { 0x0102, 0x0201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}

TEST(EndianTest, EndianSwapping32Test) {
    std::map<std::int32_t, std::int32_t> tests = {
        { 0x01020304, 0x04030201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}

TEST(EndianTest, EndianSwapping64Test) {
    std::map<std::int64_t, std::int64_t> tests = {
        { 0x0102030405060708, 0x0807060504030201 }
    };

    for (const auto& test : tests) {
        auto result = utils::swapEndian(test.first);
        EXPECT_EQ(test.second, result);
    }
}
