///////////////////////////////////////////////////////////////////////////////
/// \file Blob.hpp
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

#ifndef LUCID_BLOB_HPP
#define LUCID_BLOB_HPP

#include <ostream>
#include <vector>
#include <cstdint>
#include "Blob.hpp"

namespace lucid {

///////////////////////////////////////////////////////////////////////////////
/// \brief Class for storing pixel blobs
class Blob final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Blob();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Blob() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other Object to be copy constructed from
    Blob(const Blob& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other Object to be move constructed from
    Blob(Blob&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assignment operator
    /// \param other Object to be copy assigned from
    Blob& operator=(const Blob& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other Object to be move assigned from
    Blob& operator=(Blob&& other);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other Object to be compared against
    bool operator==(const Blob& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other Object to be compared against
    bool operator!=(const Blob& other) const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets an iterator pointing to the first pixel key associated with
    /// the blob.
    /// \return A beginning iterator to the list of pixel keys associated with the
    /// blob.
    std::vector<std::uint32_t>::iterator begin() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets an iterator pointing to the last pixel key associated with
    /// the blob.
    /// \return An ending iterator to the list of pixel keys associated with the
    /// blob.
    std::vector<std::uint32_t>::iterator end() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets a const iterator pointing to the first pixel key associated
    /// with the blob.
    /// \return A const beginning iterator to the list of pixel keys associated
    /// with the blob.
    std::vector<std::uint32_t>::const_iterator cbegin() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Gets a const iterator pointing to the first pixel key associated
    /// with the blob.
    /// \return A const beginning iterator to the list of pixel keys associated
    /// withe the blob.
    std::vector<std::uint32_t>::const_iterator cend() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds the key value of the pixel to the blob
    /// \param pixel The pixel to add to the blob.
    void addPixel(const Pixel& pixel) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds the key value of the pixel to the blob
    /// \param key The pixel key to add to the blob.
    void addPixelKey(const std::uint32_t key) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds the key value of the pixel to the blob
    /// \param keys The pixels to add to the blob.
    void addPixels(const std::vector<Pixel>& pixels) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds the key value of the pixel to the blob
    /// \param keys The pixel keys to add to the blob.
    void addPixelKeys(const std::vector<std::uint32_t>& keys) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Overloaded ostream operator for the Blob class
    /// \param os The output stream
    /// \param frame The Blob to stream out
    /// \return A reference to the ostream in use
    friend std::ostream& operator<<(std::ostream& os, const Blob& blob) noexcept;

private:
    std::vector<std::uint32_t> pixelKeys_;
};

} // lucid

#endif // LUCID_BLOB_HPP

