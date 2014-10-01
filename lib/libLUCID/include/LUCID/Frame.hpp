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

    friend std::ostream& operator<<(std::ostream& os, const Frame& frame);

private:
    std::uint32_t channel_;
    std::uint32_t timeStamp_;
    std::map<std::uint32_t, Pixel> pixels_;
};


///////////////////////////////////////////////////////////////////////////////
/// \brief Overloaded ostream operator for the Frame class
/// \param os The output stream
/// \param frame The Frame to stream out
/// \return A reference to the ostream in use
std::ostream& operator<<(std::ostream& os, const Frame& frame) {
    os << "Channel: " << frame.channel_ << "\n"
        << "Time Stamp: " << frame.timeStamp_ << "\n"
        << "Number of pixels: " << frame.pixels_.size() << "\n";
    for (const auto& p : frame.pixels_) {
        os << p.second;
    }
    return os;
}

} // lucid

#endif // LUCID_FRAME_HPP

