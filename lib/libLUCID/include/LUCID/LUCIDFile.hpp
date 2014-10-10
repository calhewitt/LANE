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

#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include "../Frame.hpp"
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
std::string toString(const CompressionMode mode) noexcept;


///////////////////////////////////////////////////////////////////////////////
/// \brief Class for handling LUCID data files
class LUCIDFile final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    LUCIDFile() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor. Opens and parses the LUCID file with the given file
    /// name.
    /// \param fileName The path/name of the file to open
    LUCIDFile(const std::string& fileName) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~LUCIDFile() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    LUCIDFile(const LUCIDFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    LUCIDFile(LUCIDFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    LUCIDFile& operator=(const LUCIDFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    LUCIDFile& operator=(LUCIDFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const LUCIDFile& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const LUCIDFile& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Opens and parses the LUCID file with the given file name into
    /// the object.
    /// \param fileName The path/name of the file to open
    void read(const std::string& fileName) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the frames associated with a channel
    /// \param channelID The ID of the channel to grab from
    /// \return A vector of frames
     std::vector<Frame> getFrames(const std::uint32_t channelID) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the LUCIDFile class
    /// \param os The output stream
    /// \param file The LUCID data file to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const LUCIDFile& file) noexcept;

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

} // lucid

#endif // LUCID_LUCIDFILE_HPP
