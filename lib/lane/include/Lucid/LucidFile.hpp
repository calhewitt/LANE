///////////////////////////////////////////////////////////////////////////////
/// \file LucidFile.hpp
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

#ifndef LANE_LUCID_LUCIDFILE_HPP
#define LANE_LUCID_LUCIDFILE_HPP

#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <cstdint>
#include "../RawInputFile.hpp"
#include "../Frame.hpp"
#include "../Utils/Endian.hpp"
#include "../Utils/Misc.hpp"

namespace lane {
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
class LucidFile final : public RawInputFile {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    LucidFile() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor. Opens and parses the LUCID file with the given file
    /// name.
    /// \param fileName The path/name of the file to open
    LucidFile(const std::string& fileName) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~LucidFile() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    LucidFile(const LucidFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    LucidFile(LucidFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    LucidFile& operator=(const LucidFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    LucidFile& operator=(LucidFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const LucidFile& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const LucidFile& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Opens and parses the LUCID file with the given file name into
    /// the object.
    /// \param fileName The path/name of the file to open
    /// \throws std::runtime_error when it fails to parse
    virtual void read(const std::string& fileName);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the frames associated with a channel
    /// \param channelID The ID of the channel to grab from
    /// \return A vector of frames
    virtual std::vector<Frame> getFrames(const std::uint32_t channelID) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the channel to frame map
    /// \return A map of channel IDs to frames
    virtual std::map<std::uint32_t, std::vector<Frame>> getChannelToFramesMap() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the start time
    /// \return The start time
    virtual std::uint32_t getStartTime() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the file ID of the configuration used
    /// \return The file ID of the configuration used
    virtual std::string getFileID() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the LucidFile class
    /// \param os The output stream
    /// \param file The LUCID data file to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const LucidFile& file) noexcept;

private:
    void clear() noexcept;

    std::array<bool, 5> chipActive_;
    std::uint32_t startTime_;
    std::string fileID_;
    std::uint32_t shutterRate_;
    bool isCompressed_;
    bool isLinearLUT_;
    CompressionMode compressionMode_;
    std::map<std::uint32_t, std::vector<Frame>> channels_;
};

} // lucid
} // lane

#endif // LANE_LUCID_LUCIDFILE_HPP
