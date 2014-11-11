///////////////////////////////////////////////////////////////////////////////
/// \file RawInputFile.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Virtual base class to inherit from for writing a new input format
/// handling module
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef LANE_RAWINPUTFILE_HPP
#define LANE_RAWINPUTFILE_HPP

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include "Frame.hpp"

namespace lane {

///////////////////////////////////////////////////////////////////////////////
/// \brief Pure virtual base class for handling raw data files
class RawInputFile {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    virtual ~RawInputFile() noexcept {}

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Opens and parses the input file with the given file name into
    /// the object.
    /// \param fileName The path/name of the file to open
    virtual void read(const std::string& fileName) = 0;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the frames associated with a channel
    /// \param channelID The ID of the channel to grab from
    /// \return A vector of frames
    virtual std::vector<Frame> getFrames(const std::uint32_t channelID) const noexcept = 0;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the channel to frame map
    /// \return A map of channel IDs to frames
    virtual std::map<std::uint32_t, std::vector<Frame>> getChannelToFramesMap() const noexcept = 0;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the start time
    /// \return The start time
    virtual std::uint32_t getStartTime() const noexcept = 0;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the file ID
    /// \return The file ID
    virtual std::string getFileID() const noexcept = 0;
};

} // lane

#endif // LANE_RAWINPUTFILE_HPP
