///////////////////////////////////////////////////////////////////////////////
/// \file BlobFinder.hpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Blob finding/building procedures
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef LUCID_BLOBFINDER_HPP
#define LUCID_BLOBFINDER_HPP

#include <vector>
#include <cstdint>
#include "Frame.hpp"
#include "Blob.hpp"

namespace lucid {

///////////////////////////////////////////////////////////////////////////////
/// \brief Analyses a given frame to find blobs of pixels.
/// Uses a simple neighbouring algorithm to group pixels.
/// \param frame The frame to analyse for blobs
/// \param threshold The count value above which to consider for blobbing.
/// Useful for filtering out noise from blobbing. Defaults to 1.
/// \return A vector/list of found blobs
std::vector<Blob> findBlobs(
    const Frame& frame,
    const unsigned int threshold = 1
) noexcept;

} // lucid

#endif // LUCID_BLOBFINDER_HPP
