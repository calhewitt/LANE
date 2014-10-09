///////////////////////////////////////////////////////////////////////////////
/// \file Pixel.cpp
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

#include <ostream>
#include <cstdint>
#include "Pixel.hpp"

namespace lucid {

Pixel::Pixel(
    const std::uint32_t x,
    const std::uint32_t y,
    const std::uint32_t c
) noexcept
: x_(x), y_(y), c_(c) {
}

Pixel::~Pixel() noexcept = default;

Pixel::Pixel(const Pixel& other) noexcept = default;

Pixel::Pixel(Pixel&& other) noexcept = default;

Pixel& Pixel::operator=(const Pixel& other) noexcept = default;

Pixel& Pixel::operator=(Pixel&& other) noexcept = default;

bool Pixel::operator==(const Pixel& other) const noexcept {
    if (this != &other) {
        return (
            x_ == other.x_ &&
            y_ == other.y_ &&
            c_ == other.c_
        );
    }

    return true;
}

bool Pixel::operator!=(const Pixel& other) const noexcept {
    return !(*this == other);
}

void Pixel::setX(const std::uint32_t x) noexcept {
    x_ = x;
}

void Pixel::setY(const std::uint32_t y) noexcept {
    y_ = y;
}

void Pixel::setC(const std::uint32_t c) noexcept {
    c_ = c;
}

std::uint32_t Pixel::getX() const noexcept {
    return x_;
}

std::uint32_t Pixel::getY() const noexcept {
    return y_;
}

std::uint32_t Pixel::getC() const noexcept {
    return c_;
}

std::ostream& operator<<(std::ostream& os, const Pixel& pixel) noexcept {
    os << "X: " << pixel.x_ << "\n"
        << "Y: " << pixel.y_ << "\n"
        << "C: " << pixel.c_ << "\n";
    return os;
}

} // lucid
