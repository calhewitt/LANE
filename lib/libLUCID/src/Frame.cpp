///////////////////////////////////////////////////////////////////////////////
/// \file Frame.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Frame storage class
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <ostream>
#include <cstdint>
#include "Frame.hpp"
#include "Pixel.hpp"

namespace lane {

Frame::Frame() noexcept
: channel_(0),
  timeStamp_(0),
  timeStampSub_(0) {
}

Frame::~Frame() noexcept = default;

Frame::Frame(const Frame& other) = default;

Frame::Frame(Frame&& other) = default;

Frame& Frame::operator=(const Frame& other) = default;

Frame& Frame::operator=(Frame&& other) = default;

bool Frame::operator==(const Frame& other) const noexcept {
    if (this != &other) {
        if(pixels_.size() == other.pixels_.size()) {
            return (
                timeStamp_ == other.timeStamp_ &&
                timeStampSub_ == other.timeStampSub_ &&
                pixels_ == other.pixels_
            );
        }

        return false;
    }

    return true;
}

bool Frame::operator!=(const Frame& other) const noexcept {
    return !(*this == other);
}

void Frame::setChannelID(const std::uint32_t channel) noexcept {
    channel_ = channel;
}

void Frame::setTimeStamp(const std::uint32_t timeStamp) noexcept {
    timeStamp_ = timeStamp;
}

void Frame::setTimeStampSub(const std::uint32_t timeStampSub) noexcept {
    timeStampSub_ = timeStampSub;
}

void Frame::setPixel(
    const std::uint32_t x,
    const std::uint32_t y,
    const std::uint32_t c
) noexcept {
    pixels_[x * 256 + y] = Pixel(x, y, c);
}

std::uint32_t Frame::getChannelID() const noexcept {
    return channel_;
}

std::uint32_t Frame::getTimeStamp() const noexcept {
    return timeStamp_;
}

std::uint32_t Frame::getTimeStampSub() const noexcept {
    return timeStampSub_;
}

Pixel Frame::getPixel(
    const std::uint32_t x,
    const std::uint32_t y
) const noexcept {
    auto result = pixels_.find(x * 256 + y);
    if (result == pixels_.end()) {
        return Pixel(0, 0, 0);
    }
    return result->second;
}

Pixel Frame::getPixel(
    const std::uint32_t key
) const noexcept {
    auto result = pixels_.find(key);
    if (result == pixels_.end()) {
        return Pixel(0, 0, 0);
    }
    return result->second;
}

const std::map<std::uint32_t, Pixel>& Frame::getPixels() const noexcept {
    return pixels_;
}

std::ostream& operator<<(std::ostream& os, const Frame& frame) noexcept {
    os << "Channel: " << frame.channel_ << "\n"
        << "Time Stamp: " << frame.timeStamp_ << "\n"
        << "Sub-second Time Stamp: " << frame.timeStampSub_ << "\n"
        << "Number of hit pixels: " << frame.pixels_.size() << "\n";
    for (const auto& p : frame.pixels_) {
        os << p.second << "\n";
    }
    return os;
}

} // lane
