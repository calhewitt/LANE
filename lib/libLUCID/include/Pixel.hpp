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
    /// \param c The TOT count
    /// \param e The energy deposited in the pixel
    Pixel(
        const std::uint32_t x = 0,
        const std::uint32_t y = 0,
        const std::uint32_t c = 0,
        const float e = 0.0
    ) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Pixel() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    Pixel(const Pixel& other) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Pixel(Pixel&& other) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    Pixel& operator=(const Pixel& other) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Pixel& operator=(Pixel&& other) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const Pixel& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const Pixel& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the x value of the pixel
    /// \param x The x value of the pixel
    void setX(const std::uint32_t x) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the y value of the pixel
    /// \param y The y value of the pixel
    void setY(const std::uint32_t y) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the c value of the pixel
    /// \param c The c value of the pixel
    void setC(const std::uint32_t c) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the x value of the pixel
    /// \return The x value of the pixel
    std::uint32_t getX() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the y value of the pixel
    /// \return The y value of the pixel
    std::uint32_t getY() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the c value of the pixel
    /// \return The c value of the pixel
    std::uint32_t getC() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets the energy deposited in that pixel
    /// \return The energy deposited in that pixel
    float getE() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the energy deposited in that pixel
    /// \param e The energy deposited in that pixel
    void setE(const float e) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the Frame class
    /// \param os The output stream
    /// \param pixel The Pixel to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel) noexcept;

private:
    std::uint32_t x_, y_, c_;
    float  e_;
};

} // lucid

#endif // LUCID_FRAME_HPP
