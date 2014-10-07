///////////////////////////////////////////////////////////////////////////////
/// \file Frame.hpp
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

#ifndef LUCID_FRAME_HPP
#define LUCID_FRAME_HPP

#include <ostream>
#include <map>
#include <cstdint>
#include "Pixel.hpp"

namespace lucid {

///////////////////////////////////////////////////////////////////////////////
/// \brief Class for handling detector frames
class Frame final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Frame() = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Frame() = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    Frame(const Frame& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Frame(Frame&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    Frame& operator=(const Frame& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Frame& operator=(Frame&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const Frame& other) const {
        if (this != &other) {
            return (
                timeStamp_ == other.timeStamp_ &&
                timeStampSub_ == other.timeStampSub_&&
                pixels_ == other.pixels_
            );
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const Frame& other) const {
        return !(*this == other);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the channel ID of the frame
    /// \param other Object to be compared against
    void setChannelID(const std::uint32_t channel) {
        channel_ = channel;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the time stamp of the frame
    /// \param timeStamp The time stamp to set the frame's time stamp to
    void setTimeStamp(const std::uint32_t timeStamp) {
        timeStamp_ = timeStamp;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the time stamp sub second time of the frame
    /// \param timeStampSub The sub second time stamp to set the frame's sub
    /// time stamp to
    void setTimeStampSub(const std::uint32_t timeStampSub) {
        timeStampSub_ = timeStampSub;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets a pixel
    /// \param x The x value
    /// \param y The y value
    /// \param c The count value
    void setPixel(
        const std::uint32_t x,
        const std::uint32_t y,
        const std::uint32_t c
    ) {
        pixels_[x * 256 + y] = Pixel(x, y, c);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the channel ID of the frame
    /// \return The channel ID associated with this frame
    std::uint32_t getChannelID() const {
        return channel_;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the time stamp of the frame
    /// \return The time stamp associated with the frame
    std::uint32_t getTimeStamp() const {
        return timeStamp_;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the time stamp sub second time of the frame
    /// \return The time stamp sub second time associated with the frame
    std::uint32_t getTimeStampSub() const {
        return timeStampSub_;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets a pixel
    /// \param x The x value of the pixel
    /// \param y The y value of the pixel
    /// \return The pixel object containing the pixel data
    Pixel getPixel(
        const std::uint32_t x,
        const std::uint32_t y
    ) const {
        return pixels_.at(x * 256 + y);
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the Frame class
    /// \param os The output stream
    /// \param frame The Frame to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const Frame& frame);

private:
    std::uint32_t channel_;
    std::uint32_t timeStamp_;
    std::uint32_t timeStampSub_;
    std::map<std::uint32_t, Pixel> pixels_;
};

std::ostream& operator<<(std::ostream& os, const Frame& frame) {
    os << "Channel: " << frame.channel_ << "\n"
        << "Time Stamp: " << frame.timeStamp_ << "\n"
        << "Sub-second Time Stamp: " << frame.timeStampSub_ << "\n"
        << "Number of hit pixels: " << frame.pixels_.size() << "\n";
    for (const auto& p : frame.pixels_) {
        os << p.second << "\n";
    }
    return os;
}

} // lucid

#endif // LUCID_FRAME_HPP

