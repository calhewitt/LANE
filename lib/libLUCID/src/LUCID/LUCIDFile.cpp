///////////////////////////////////////////////////////////////////////////////
/// \file LUCIDFile.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Utilities for parsing LUCID detector data files into a convenient
/// data structure.
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

// TODO: Figure out matrix tables
// TODO: Figure out shutter mode 16 bits bitfield
// TODO: Clean up reading in code?

// -Header Format:-
// First 2 bytes are header magic - 0xDC 0xCC
// 1 byte - Matrix table bits [1:0] and a bit field of the active
// channel
// 1 byte - Matrix table bits [9:2]
// 2 bytes - Shutter mode
// 1 byte - compression bitfield
// 1 byte - shutter rate
// 4 bytes - start time
// 4 bytes - file ID

// -Frame Format:-
// Start of Frame demarker
// Control data word w/ timestamp
// and multiple of the following until the next time stamp:
// Control data word w/ channel marker, 5 bit bitfield indexing from 0 at 2nd
// byte
// Data payload
// Pixels are stored in 256 word rows from bottom left pixel to top right

// -Word Format:-
// 16-bit words, first 1/2 bits are flag bits, options are:
// 0  - Number of zeroes
// 10 - Payload data
// 11 - Control data - Start of frame data characters etc.

#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include "LUCID/LUCIDFile.hpp"
#include "Frame.hpp"
#include "Utils/Endian.hpp"
#include "Utils/Misc.hpp"

namespace lucid {

std::string toString(const CompressionMode mode) noexcept {
    switch (mode) {
    case CompressionMode::RLE:
        return "Run-Length Encoded";
        break;
    case CompressionMode::XYV:
        return "X, Y, Value";
        break;
    case CompressionMode::None:
        return "Not compressed";
    default:
        return "Unknown compression mode";
        break;
    }
}

LUCIDFile::LUCIDFile() noexcept
: startTime_(0),
fileID_(0),
shutterRate_(0),
isCompressed_(false),
isLinearLUT_(false),
compressionMode_(CompressionMode::Unknown),
channels_() {
    chipActive_.fill(false);
}

LUCIDFile::LUCIDFile(const std::string& fileName) noexcept
: startTime_(0),
fileID_(0),
shutterRate_(0),
isCompressed_(false),
isLinearLUT_(false),
compressionMode_(CompressionMode::Unknown),
channels_() {
    chipActive_.fill(false);
    read(fileName);
}

LUCIDFile::~LUCIDFile() noexcept = default;

LUCIDFile::LUCIDFile(const LUCIDFile& other) = default;

LUCIDFile::LUCIDFile(LUCIDFile&& other) = default;

LUCIDFile& LUCIDFile::operator=(const LUCIDFile& other) = default;

LUCIDFile& LUCIDFile::operator=(LUCIDFile&& other) = default;

bool LUCIDFile::operator==(const LUCIDFile& other) const noexcept {
    if (this != &other) {
        if(channels_.size() == other.channels_.size()) {
            return (
                chipActive_ == other.chipActive_ &&
                startTime_ == other.startTime_ &&
                fileID_ == other.fileID_ &&
                shutterRate_ == other.shutterRate_ &&
                isCompressed_ == other.isCompressed_ &&
                isLinearLUT_ == other.isLinearLUT_ &&
                compressionMode_ == other.compressionMode_ &&
                channels_ == other.channels_
            );
        }

        return false;
    }

    return true;
}

bool LUCIDFile::operator!=(const LUCIDFile& other) const noexcept {
    return !(*this == other);
}

void LUCIDFile::read(const std::string& fileName) noexcept {
    clear();

    std::ifstream input(fileName, std::ios::binary | std::ios::ate);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
    
    // Get file size
    const std::uint64_t fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    // Read into heap array
    std::vector<std::uint8_t> data;
    data.reserve(fileSize);
    input.read(reinterpret_cast<char*>(&data[0]), fileSize);
    
    input.close();

    // LUCID is big endian
    const bool isLittleEndian = utils::isLittleEndian();

    // Integrity checks
    // Check the file magic
    if (data[0] != 0xDC && data[1] != 0xCC) {
        throw std::runtime_error(
            "'" + fileName + "' is not a LUCID data file"
        );
    }
    // Check that the file size is big enough for the header
    if (fileSize < 16) {
        throw std::runtime_error(
            "'" + fileName + "' is not a valid LUCID data file"
        );
    }

    // Channel
    // The active detectors are flagged using a bit field.
    // This is only the last 5 bits of the byte.
    // 1F would translate to 00011111
    // This would mean all 5 detectors are taking data for this run.
    // I do not know which way round the bit field is
    // so whether TPX0 is bit 3 or bit 7 is unknown as of yet
    for (unsigned int i = 0; i < 5; ++i) {
        chipActive_[i] = (data[2] >> i) ? true : false;
    }

    // Compression
    // Type of compression
    // 0 - RLE
    // 1 - XYV
    switch ((data[6] >> 2) & 0x01) {
    case 0:
        compressionMode_ = CompressionMode::RLE;
        break;
    case 1:
        compressionMode_ = CompressionMode::XYV;
        break;
    }
    // Type of LUT used
    // 0 - PRN
    // 1 - Linear
    isLinearLUT_ = ((data[6] >> 1) & 0x01) ? true : false;
    // Data compression enabled?
    // 0 - Off
    // 1 - On
    isCompressed_ = (data[6] & 0x01) ? true : false;
    if (!isCompressed_) {
        compressionMode_ = CompressionMode::None;
    }

    // Shutter Rate
    shutterRate_ = static_cast<std::uint32_t>(data[7]);

    // Start Time
    startTime_ = *(reinterpret_cast<std::uint32_t*>(&data[8]));

    // File ID
    fileID_ = *(reinterpret_cast<std::uint32_t*>(&data[12]));

    if (isLittleEndian) {
        startTime_ = utils::swapEndian(startTime_);
        fileID_ = utils::swapEndian(fileID_);
    }

    // Read in frames
    bool isFindingFrames = true;
    std::uint64_t index = 16;
    while (isFindingFrames) {
        // Check integrity for demarker and timestamp
        if (index + 7 > fileSize) {
            throw std::runtime_error("Malformed data file - Unexpected end of file in frame");
        }

        // Check for frames demarkers
        if (!(data[index] == 0xDC && data[index + 1] == 0xDF)) {
            throw std::runtime_error("Malformed data file - Expected frame header");
        }

        // Get timestamp - 4 byte UNIX time, 1 byte sub second time
        std::uint32_t frameTime = *(reinterpret_cast<std::uint32_t*>(&data[index + 2]));
        if (isLittleEndian) {
            frameTime = utils::swapEndian(frameTime);
        }
        std::uint32_t frameTimeSub = *(reinterpret_cast<std::uint8_t*>(&data[index + 6]));

        // Move forwards 7 bytes
        index += 7;

        bool isFindingChannelData = true;
        while (isFindingChannelData) {
            if (index + 1 > fileSize - 1) {
                throw std::runtime_error("Malformed data file - Unexpected end of file in channel header");
            }

            // Get channel
            std::uint32_t channel = 5;
            if ((data[index] >> 6) == 0x03) {
                if ((data[index] & 0x01) == 1) {
                    channel = 0;
                } else if (((data[index] & 0x02) >> 1) == 1) {
                    channel = 1;
                } else if (((data[index] & 0x04) >> 2) == 1) {
                    channel = 2;
                } else if (((data[index] & 0x08) >> 3) == 1) {
                    channel = 3;
                } else if (((data[index] & 0x16) >> 4) == 1) {
                    channel = 4;
                }
            } else {
                throw std::runtime_error("Malformed data file - Expected channel control word");
            }

            // Move past the channel marker
            index += 1;

            // Xs and Ys starting from the bottom left of the pixel grid
            std::uint8_t x = 0;
            std::uint8_t y = 0;
            // Create the frame
            auto currentFrame = Frame();
            currentFrame.setTimeStamp(frameTime);
            currentFrame.setTimeStampSub(frameTimeSub);
            currentFrame.setChannelID(channel);
            // Get data until next control word (channel marker or frame demarking)
            bool isFindingData = true;
            while (isFindingData) {
                // Check if this is the last word, and stop reading after this iteration if so
                if (index + 2 >= fileSize - 1) {
                    isFindingData = false;
                    isFindingChannelData = false;
                    isFindingFrames = false;
                // Check the next word for a new frame demarker
                } else if (data[index + 2] == 0xDC && data[index + 3] == 0xDF) {
                    isFindingData = false;
                    isFindingChannelData = false;
                // Check for a channel marker
                } else if ((data[index + 2] >> 6) == 0x03) {
                    isFindingData = false;
                }

                // Get actual data
                if ((data[index] >> 6) == 0x02) {
                    // Payload data (10-)
                    std::uint16_t payload = *(reinterpret_cast<std::uint16_t*>(&data[index]));
                    if (isLittleEndian) {
                        payload = utils::swapEndian(payload);
                    }
                    payload = payload & static_cast<std::uint16_t>(32767);
                    if (x == 255) {
                        x = 0;
                        ++y;
                    } else {
                        ++x;
                    }

                    if (payload > 0) {
                        currentFrame.setPixel(x, y, payload);
                    }
                } else if ((data[index] >> 7) == 0x00) {
                    // Number of zeroes (0-)
                    std::uint16_t numberOfZeroes = *(reinterpret_cast<std::uint16_t*>(&data[index]));
                    for (std::uint16_t i = 0; i < numberOfZeroes; ++i) {
                        if (x == 255) {
                            x = 0;
                            ++y;
                        } else {
                            ++x;
                        }
                    }
                }

                index += 2;
            }

            // Add frame to channel map
            channels_[channel].emplace_back(currentFrame);
        }
    }
}

std::vector<Frame> LUCIDFile::getFrames(const std::uint32_t channelID) const noexcept {
    assert(channelID <= 4);
    return channels_.find(channelID)->second;
}

std::map<std::uint32_t, std::vector<Frame>> LUCIDFile::getChannelToFramesMap() const noexcept {
    return channels_;
}

std::uint32_t LUCIDFile::getStartTime() const noexcept {
    return startTime_;
}

std::uint32_t LUCIDFile::getFileID() const noexcept {
    return fileID_;
}

void LUCIDFile::clear() noexcept {
    chipActive_.fill(false);
    startTime_ = 0;
    fileID_ = 0;
    shutterRate_ = 0;
    isCompressed_ = false;
    isLinearLUT_ = false;
    compressionMode_ = CompressionMode::Unknown;
    channels_.clear();
}

std::ostream& operator<<(std::ostream& os, const LUCIDFile& file) noexcept {
    os << "-HEADER-\n"
        << "File ID: " << file.fileID_ << "\n"
        << "Start Time: " << file.startTime_ << "\n";
    os << "Active Chips:\n";
    for (unsigned int i = 0; i <= 4; ++i) {
        os << "Chip " << i << " - "
            << (file.chipActive_[i] ? "Active" : "Inactive") << "\n";
    }
    os << "Shutter Rate: "
            << file.shutterRate_ << "\n"
        << "Is Compressed?: "
            << (file.isCompressed_ ? "True" : "False") << "\n"
        << "Compression Mode: "
            << toString(file.compressionMode_) << "\n"
        << "Uses Linear LUT?: "
            << (file.isLinearLUT_ ? "True" : "False") << "\n";
    for (const auto& channel : file.channels_) {
        for (const auto& frame : channel.second) {
            os << frame;
        }
    }
    return os;
}

} // lucid
