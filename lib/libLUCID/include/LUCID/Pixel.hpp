///////////////////////////////////////////////////////////////////////////////
/// \file Pixel.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Pixel storage class
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef LUCID_PIXEL_HPP
#define LUCID_PIXEL_HPP

#include <ostream>
#include <cstdint>

namespace lucid {

///////////////////////////////////////////////////////////////////////////////
/// \brief Class for storing detector pixels
class Pixel final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    /// \param x The x coordinate
    /// \param y The y coordinate
    /// \param c The energy count
    Pixel(
        const std::uint32_t x = 0,
        const std::uint32_t y = 0,
        const std::uint32_t c = 0
    )
    : x_(x), y_(y), c_(c) {
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Pixel() = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    Pixel(const Pixel& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Pixel(Pixel&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    Pixel& operator=(const Pixel& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Pixel& operator=(Pixel&& other) = default;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const Pixel& other) const {
        if (this != &other) {
            return (
                x_ == other.x_ &&
                y_ == other.y_ &&
                c_ == other.c_
            );
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const Pixel& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel);

private:
    std::uint32_t x_, y_, c_;
};


///////////////////////////////////////////////////////////////////////////////
/// \brief Overloaded ostream operator for the Frame class
/// \param os The output stream
/// \param pixel The Pixel to stream out
/// \return A reference to the ostream in use
std::ostream& operator<<(std::ostream& os, const Pixel& pixel) {
    os << "X: " << pixel.x_ << "\n"
        << "Y: " << pixel.y_ << "\n"
        << "C: " << pixel.c_ << "\n";
    return os;
}

} // lucid

#endif // LUCID_FRAME_HPP

