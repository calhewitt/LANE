///////////////////////////////////////////////////////////////////////////////
/// \file LaneFile.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Handles the intermediate file format for LANE raw data
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef LANE_LANEFILE_HPP
#define LANE_LANEFILE_HPP

#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <cstdint>
#include "Frame.hpp"
#include "Utils/Misc.hpp"

namespace lane {

///////////////////////////////////////////////////////////////////////////////
/// \brief Class for handling LANE intermedaite raw data files
class LaneFile final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    LaneFile();
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the LaneFile class
    /// \param fileName The name/path of the file to read in
    LaneFile(const std::string& fileName);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~LaneFile() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    LaneFile(const LaneFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    LaneFile(LaneFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    LaneFile& operator=(const LaneFile& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    LaneFile& operator=(LaneFile&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const LaneFile& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const LaneFile& other) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Reads in a LANE intermediate file
    /// \param fileName The name/path of the file to read in
    void read(const std::string& fileName);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Writes out the stored information to a LANE intermediate file
    /// \param fileName The name/path of the file to write to
    void write(const std::string& fileName);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds a frame object to internal storage
    /// \param frame A frame to add to the internal storage
    /// \param channelID The channelID to associate to the frame
    void addFrame(
        const Frame& frame,
        std::uint32_t channelID = 0
    ) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the frames associated with a channel
    /// \param channelID The ID of the channel to grab from
    /// \return A vector of frames
    std::vector<Frame> getFrames(
        const std::uint32_t channelID
    ) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the file ID
    /// \param fileID The file ID to set
    void setFileID(const std::string& fileID) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the start time
    /// \param startTime The start time to set
    void setStartTime(const std::uint32_t startTime) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the file ID of the configuration used
    /// \return The file ID of the configuration to associated with this file
    std::string getFileID() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the start time
    /// \return The start time associated to this file
    std::uint32_t getStartTime() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the LaneFile class
    /// \param os The output stream
    /// \param file The LANE data file to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(
        std::ostream& os,
        const LaneFile& file
    ) noexcept;

private:
    void clear() noexcept;

    std::map<std::uint32_t, std::vector<Frame>> channels_;
    std::uint32_t startTime_;
    std::string fileID_;
};

} // lane

#endif // LANE_LANEFILE_HPP
