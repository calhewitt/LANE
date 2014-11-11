///////////////////////////////////////////////////////////////////////////////
/// \file Blob.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Blob storage class
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <ostream>
#include <vector>
#include <cstdint>
#include "Pixel.hpp"
#include "Blob.hpp"

namespace lane {

Blob::Blob() = default;

Blob::~Blob() noexcept = default;

Blob::Blob(const Blob& other) = default;

Blob::Blob(Blob&& other) = default;

Blob& Blob::operator=(const Blob& other) = default;

Blob& Blob::operator=(Blob&& other) = default;

bool Blob::operator==(const Blob& other) const noexcept {
    if (this != &other) {
        if(pixelKeys_.size() == other.pixelKeys_.size()) {
            return (
                pixelKeys_ == other.pixelKeys_
            );
        }

        return false;
    }

    return true;
}

bool Blob::operator!=(const Blob& other) const noexcept {
    return !(*this == other);
}

std::vector<std::uint32_t>::iterator
Blob::begin() noexcept {
    return pixelKeys_.begin();
}

std::vector<std::uint32_t>::iterator
Blob::end() noexcept {
    return pixelKeys_.end();
}

std::vector<std::uint32_t>::const_iterator
Blob::begin() const noexcept {
    return pixelKeys_.begin();
}

std::vector<std::uint32_t>::const_iterator
Blob::end() const noexcept {
    return pixelKeys_.end();
}

void Blob::addPixel(const Pixel& pixel) noexcept {
    pixelKeys_.emplace_back(pixel.getX() * 256 + pixel.getY());
}

void Blob::addPixelKey(const std::uint32_t key) noexcept {
    pixelKeys_.emplace_back(key);
}

void Blob::addPixels(const std::vector<Pixel>& pixels) noexcept {
    for (const auto& p : pixels) {
        pixelKeys_.emplace_back(p.getX() * 256 + p.getY());
    }
}

void Blob::addPixelKeys(const std::vector<std::uint32_t>& keys) noexcept {
    for (const auto& k : keys) {
        pixelKeys_.emplace_back(k);
    }
}

std::ostream& operator<<(std::ostream& os, const Blob& blob) noexcept {
    os << "Blob keys:\n";
    for (const auto& key : blob.pixelKeys_) {
        os << key << ", ";
    }
    return os;
}

} // lane
