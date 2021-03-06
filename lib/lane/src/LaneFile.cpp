///////////////////////////////////////////////////////////////////////////////
/// \file LaneFile.cpp
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

#include <ostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include "LaneFile.hpp"
#include "Frame.hpp"
#include "Pixel.hpp"
#include "Utils/Misc.hpp"

namespace {

std::vector<std::string>& split(
    const std::string& s,
    char delim,
    std::vector<std::string>& elems
) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

}


namespace lane {

LaneFile::LaneFile() = default;

LaneFile::LaneFile(const std::string& fileName)
: startTime_(0),
fileID_("") {
    read(fileName);
}

LaneFile::~LaneFile() noexcept = default;

LaneFile::LaneFile(const LaneFile& other) = default;

LaneFile::LaneFile(LaneFile&& other) = default;

LaneFile& LaneFile::operator=(const LaneFile& other) = default;

LaneFile& LaneFile::operator=(LaneFile&& other) = default;

bool LaneFile::operator==(const LaneFile& other) const noexcept {
    if (this != &other) {
        if(channels_.size() == other.channels_.size()) {
            return (
                fileID_ == other.fileID_ &&
                startTime_ == other.startTime_ &&
                channels_ == other.channels_
            );
        }

        return false;
    }

    return true;
}

bool LaneFile::operator!=(const LaneFile& other) const noexcept {
    return !(*this == other);
}


// Currently in the following format:
// ID,STARTTIME
// CHANNELID
// FRAMETIMESTAMP
// X,Y,C
// EOF
// EOC
// CHANNELID 
// ...etc
void LaneFile::read(const std::string& fileName) {
    clear();
    std::ifstream input(
        fileName,
        std::ios::binary | std::ios::in
    );
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
    
    std::string buffer = "";
    std::vector<std::string> splitElems;
    
    std::getline(input, buffer);
    split(buffer, ',', splitElems);
    // Get the file ID
    fileID_ = splitElems[0];
    // Get the start time
    startTime_ = std::stoi(splitElems[1]);
    splitElems.clear();
    
    while (std::getline(input, buffer)) {
        // Get channel ID
        std::uint32_t currentChannelID = 0;
        currentChannelID = std::stoi(buffer);
        
        // Get Frames for a channel
        while (std::getline(input, buffer)) {
            if (input.eof() || buffer == "EOC") {
                break;
            }
            Frame currentFrame;
            currentFrame.setChannelID(currentChannelID);
            
            // Get frame timestamp
            split(buffer, '.', splitElems);
            currentFrame.setTimeStamp(std::stoi(splitElems[0]));
            currentFrame.setTimeStampSub(std::stoi(splitElems[1]));
            splitElems.clear();

            // Get pixels
            while (std::getline(input, buffer)) {
                if (input.eof() || buffer == "EOF") {
                    break;
                }
                
                split(buffer, ',', splitElems);
                std::uint32_t x = std::stoi(splitElems[0]);
                std::uint32_t y = std::stoi(splitElems[1]);
                std::uint32_t c = std::stoi(splitElems[2]);
                splitElems.clear();
                
                currentFrame.setPixel(x, y, c);
            }
            
            addFrame(currentFrame, currentChannelID);
        }
    }
}

void LaneFile::write(const std::string& fileName) {
    if (channels_.size() == 0) {
        return; // exit early if the internal data is empty
    }
    
    std::ofstream output(
        fileName,
        std::ios::trunc | std::ios::binary | std::ios::out
    );
    if (!output.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
    
    // Write out the data to a file
    output << fileID_ << "," << startTime_ << "\n";
    for (const auto& channel : channels_) {
        output << channel.first << "\n";
        for (const auto& frame : channel.second) {
            output << frame.getTimeStamp() << "." << frame.getTimeStampSub() << "\n";
            for (const auto& pixel : frame.getPixels()) {
                output << pixel.second.getX() << ","
                    << pixel.second.getY() << ","
                    << pixel.second.getC() << "\n";
            }
            output << "EOF\n"; // End of frame demarcation
        }
        output << "EOC\n"; // End of channel demarcation
    }
}

void LaneFile::addFrame(
    const Frame& frame,
    const std::uint32_t channelID
) noexcept {
    channels_[channelID].emplace_back(frame);
}

std::vector<Frame> LaneFile::getFrames(
    const std::uint32_t channelID
) const noexcept {
    return channels_.find(channelID)->second;
}

void LaneFile::setFileID(const std::string& fileID) noexcept {
    fileID_ = fileID;
}

void LaneFile::setStartTime(const std::uint32_t startTime) noexcept {
    startTime_ = startTime;
}

std::string LaneFile::getFileID() const noexcept {
    return fileID_;
}

std::uint32_t LaneFile::getStartTime() const noexcept {
    return startTime_;
}

void LaneFile::clear() noexcept {
    channels_.clear();
}

std::ostream& operator<<(std::ostream& os, const LaneFile& file) noexcept {
    // TODO
    return os;
}

} // lane
