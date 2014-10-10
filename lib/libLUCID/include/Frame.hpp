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
    Frame() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Frame() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    Frame(const Frame& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Frame(Frame&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    Frame& operator=(const Frame& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Frame& operator=(Frame&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const Frame& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const Frame& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the channel ID of the frame
    /// \param channel The channel ID to use
    void setChannelID(const std::uint32_t channel) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the time stamp of the frame
    /// \param timeStamp The time stamp to set the frame's time stamp to
    void setTimeStamp(const std::uint32_t timeStamp) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the time stamp sub second time of the frame
    /// \param timeStampSub The sub second time stamp to set the frame's sub
    /// time stamp to
    void setTimeStampSub(const std::uint32_t timeStampSub) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets a pixel
    /// \param x The x value
    /// \param y The y value
    /// \param c The count value
    void setPixel(
        const std::uint32_t x,
        const std::uint32_t y,
        const std::uint32_t c
    ) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the channel ID of the frame
    /// \return The channel ID associated with this frame
    std::uint32_t getChannelID() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the time stamp of the frame
    /// \return The time stamp associated with the frame
    std::uint32_t getTimeStamp() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the time stamp sub second time of the frame
    /// \return The time stamp sub second time associated with the frame
    std::uint32_t getTimeStampSub() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets a pixel
    /// \param x The x value of the pixel
    /// \param y The y value of the pixel
    /// \return The pixel object containing the pixel data
    Pixel getPixel(
        const std::uint32_t x,
        const std::uint32_t y
    ) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets a constant reference to all the pixels
    /// \return A constant reference to all the pixels
    const std::map<std::uint32_t, Pixel>& getPixels() const noexcept;

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the Frame class
    /// \param os The output stream
    /// \param frame The Frame to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const Frame& frame) noexcept;

private:
    std::uint32_t channel_;
    std::uint32_t timeStamp_;
    std::uint32_t timeStampSub_;
    std::map<std::uint32_t, Pixel> pixels_;
};

} // lucid

#endif // LUCID_FRAME_HPP
