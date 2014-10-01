///////////////////////////////////////////////////////////////////////////////
/// \file LUCIDFile.hpp
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

#ifndef LUCID_LUCIDFILE_HPP
#define LUCID_LUCIDFILE_HPP

// TODO: Figure out matrix tables
// TODO: Figure out shutter mode 16 bits bitfield
// TODO: Implement frame reading
// TODO: provide public accessors

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
// Start of Frame
// Header
// Control data word w/ timestamp
// and multiple of the following until the next time stamp:
// Control data word w/ channel marker, 5 bit bitfield indexing from 0 at 2nd
// byte
// Data payload

// -Word Format:-
// 16-bit words, first 1/2 bits are flag bits, options are:
// 0  - Number of zeroes
// 10 - Payload data
// 11 - Control data - Start of frame data characters etc.
//

#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <cstdint>
#include "Frame.hpp"
#include "../Utils/Endian.hpp"
#include "../Utils/Misc.hpp"


namespace lucid {

///////////////////////////////////////////////////////////////////////////////
/// \brief List of possible compression modes
enum class CompressionMode {
    RLE,
    XYV,
    None,
    Unknown,
};

///////////////////////////////////////////////////////////////////////////////
/// \brief Converts a compression mode to a string
/// \param mode The compression mode type to convert to a string
/// \return The string version of the compression mode
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


///////////////////////////////////////////////////////////////////////////////
/// \brief Class for handling LUCID data files
class LUCIDFile final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    LUCIDFile()
    : startTime(0),
    fileID(0),
    shutterRate(0),
    isCompressed(false),
    isLinearLUT(false),
    compressionMode(CompressionMode::Unknown),
    channels_() {
        chipActive.fill(false);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor. Opens and parses the LUCID file with the given file
    /// name.
    /// \param fileName The path/name of the file to open
    LUCIDFile(const std::string& fileName)
    : startTime(0),
    fileID(0),
    shutterRate(0),
    isCompressed(false),
    isLinearLUT(false),
    compressionMode(CompressionMode::Unknown),
    channels_() {
        chipActive.fill(false);
        read(fileName);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~LUCIDFile() = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    LUCIDFile(const LUCIDFile& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    LUCIDFile(LUCIDFile&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    LUCIDFile& operator=(const LUCIDFile& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    LUCIDFile& operator=(LUCIDFile&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const LUCIDFile& other) {
        if (this != &other) {
            return (
                chipActive == other.chipActive &&
                startTime == other.startTime &&
                fileID == other.fileID &&
                shutterRate == other.shutterRate &&
                isCompressed == other.isCompressed &&
                isLinearLUT == other.isLinearLUT &&
                compressionMode == other.compressionMode &&
                channels_ == other.channels_
            );
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const LUCIDFile& other) {
        return !(*this == other);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Opens and parses the LUCID file with the given file name into
    /// the object.
    /// \param fileName The path/name of the file to open
    void read(const std::string& fileName) {
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
        if (fileSize < 15) {
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
            chipActive[i] = (data[2] >> i) ? true : false;
        }

        // Compression
        // Type of compression
        // 0 - RLE
        // 1 - XYV
        switch ((data[6] >> 2) & 0x01) {
        case 0:
            compressionMode = CompressionMode::RLE;
            break;
        case 1:
            compressionMode = CompressionMode::XYV;
            break;
        }
        // Type of LUT used
        // 0 - PRN
        // 1 - Linear
        isLinearLUT = ((data[6] >> 1) & 0x01) ? true : false;
        // Data compression enabled?
        // 0 - Off
        // 1 - On
        isCompressed = (data[6] & 0x01) ? true : false;
        if (!isCompressed) {
            compressionMode = CompressionMode::None;
        }

        // Shutter Rate
        shutterRate = static_cast<std::uint32_t>(data[7]);

        // Start Time
        startTime = *(reinterpret_cast<std::uint32_t*>(&data[8]));

        // File ID
        fileID = *(reinterpret_cast<std::uint32_t*>(&data[12]));

        if (isLittleEndian) {
            startTime = utils::swapEndian(startTime);
            fileID = utils::swapEndian(fileID);
        }

        // Read in frames
        //OTHER STUFF FIRST
        //
        // Check for control word
        bool isFindingFrames = true;
        std::uint64_t index = 16;
        while(isFindingFrames) {
            if ((data[index] >> 6) == 3) {
                // Get channel
                std::uint32_t channel = 6;
                if (data[index + 2] >> 6 == 3) {
                    if ((data[index + 2] & 0x01) == 1) {
                        channel = 1;
                    } else if (((data[index + 2] & 2) >> 1) == 1) {
                        channel = 2;
                    } else if (((data[index + 2] & 4) >> 1) == 1) {
                        channel = 3;
                    } else if (((data[index + 2] & 8) >> 1) == 1) {
                        channel = 4;
                    } else if (((data[index + 2] & 16) >> 1) == 1) {
                        channel = 5;
                    }
                }
                else {
                    throw std::runtime_error("Malformed data file");
                }
                // Get earlier time stamp

                // Get data

                // Rinse and repeat
                //index += 2;
                if (index < fileSize) {
                    isFindingFrames = false;
                }
            } else {
                throw std::runtime_error("Malformed data file");
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const LUCIDFile& file);

private:
    std::array<bool, 5> chipActive;
    std::uint32_t startTime;
    std::uint32_t fileID;
    std::uint32_t shutterRate;
    bool isCompressed;
    bool isLinearLUT;
    CompressionMode compressionMode;
    std::map<std::uint32_t, std::vector<Frame>> channels_;
};


///////////////////////////////////////////////////////////////////////////////
/// \brief Overloaded ostream operator for the LUCIDFile class
/// \param os The output stream
/// \param file The LUCIDFile to stream out
/// \return A reference to the ostream in use
std::ostream& operator<<(std::ostream& os, const LUCIDFile& file) {
    os << "-HEADER-\n"
        << "File ID: " << file.fileID << "\n"
        << "Start Time: " << file.startTime << "\n";
    os << "Active Chips:\n";
    for (unsigned int i = 0; i < 5; ++i) {
        os << "Chip " << i << " - "
            << (file.chipActive[i] ? "Active" : "Inactive") << "\n";
    }
    os << "Shutter Rate: "
            << file.shutterRate << "\n"
        << "Is Compressed?: "
            << (file.isCompressed ? "True" : "False") << "\n"
        << "Compression Mode: "
            << toString(file.compressionMode) << "\n"
        << "Uses Linear LUT?: "
            << (file.isLinearLUT ? "True" : "False") << "\n";
    for (const auto& channel : file.channels_) {
        os << "Channel: " << channel.first << "\n";
        for (const auto& frame : channel.second) {
            os << frame;
        }
    }
    return os;
}

} // lucid

#endif // LUCID_LUCIDFILE_HPP
