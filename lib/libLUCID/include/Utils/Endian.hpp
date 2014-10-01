///////////////////////////////////////////////////////////////////////////////
/// \file Endian.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief An endian handling class
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef UTILS_ENDIAN_HPP
#define UTILS_ENDIAN_HPP

#include <cstdint>

namespace utils {

///////////////////////////////////////////////////////////////////////////////
/// \brief Checks if the system is big endian
/// \return A boolean stating true if the system is big endian
inline bool isBigEndian() noexcept {
    union {
        std::uint16_t i;
        char c[2];
    } data = { 0x0102 };
    
    return data.c[0] == 0x01;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Checks if the system is little endian
/// \return A boolean stating true if the system is little endian
inline bool isLittleEndian() noexcept {
    union {
        std::uint16_t i;
        char c[2];
    } data = { 0x0102 };
    
    return data.c[0] == 0x02;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::uint16_t swapEndian(const std::uint16_t value) noexcept {
	return (value << 8) | (value >> 8);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::uint32_t swapEndian(const std::uint32_t value) noexcept {
	return (value << 24) | ((value << 8) & 0x00ff0000) | ((value >> 8) & 0x0000ff00) | (value >> 24);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::uint64_t swapEndian(const std::uint64_t value) noexcept {
	std::uint64_t swapped;
    swapped = (swapped & 0x00000000FFFFFFFF) << 32 | (swapped & 0xFFFFFFFF00000000) >> 32;
    swapped = (swapped & 0x0000FFFF0000FFFF) << 16 | (swapped & 0xFFFF0000FFFF0000) >> 16;
    swapped = (swapped & 0x00FF00FF00FF00FF) << 8  | (swapped & 0xFF00FF00FF00FF00) >> 8;
    return swapped;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::int16_t swapEndian(const std::int16_t value) noexcept {
	return (value << 8) | (value >> 8);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::int32_t swapEndian(const std::int32_t value) noexcept {
	return (value << 24) | ((value << 8) & 0x00ff0000) | ((value >> 8) & 0x0000ff00) | (value >> 24);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Swaps the endianness of the input
/// \param value The value to be endian swapped
/// \return The swapped value
inline std::int64_t swapEndian(const std::int64_t value) noexcept {
	std::uint64_t swapped;
    swapped = (swapped & 0x00000000FFFFFFFF) << 32 | (swapped & 0xFFFFFFFF00000000) >> 32;
    swapped = (swapped & 0x0000FFFF0000FFFF) << 16 | (swapped & 0xFFFF0000FFFF0000) >> 16;
    swapped = (swapped & 0x00FF00FF00FF00FF) << 8  | (swapped & 0xFF00FF00FF00FF00) >> 8;
    return swapped;
}

} // utils

#endif // UTILS_ENDIAN_HPP
