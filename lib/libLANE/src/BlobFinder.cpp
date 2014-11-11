///////////////////////////////////////////////////////////////////////////////
/// \file BlobFinder.cpp
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

#include <vector>
#include <cstdint>
#include "Pixel.hpp"
#include "Blob.hpp"
#include "BlobFinder.hpp"

namespace lane {

namespace {

// Returns a list of all the pixels neighbouring the given pixel
// threshold determines the minimum value including and above which the 
// neighbour should be considered
std::vector<Pixel> getNeighbours(
    const Frame& frame,
    const Pixel& pixel
) noexcept {
    std::vector<Pixel> neighbours;
    
    // Iterate over all possible directions
    // whilst checking for x/y values outside of the possible number of pixels
    std::vector<int> xDirs { -1, -1, -1,  0, 0,  1, 1, 1 };
    std::vector<int> yDirs { -1,  0,  1, -1, 1, -1, 0, 1 };
    for (unsigned int i = 0; i < xDirs.size() - 1; ++i) {
        auto px = pixel.getX();
        auto py = pixel.getY();
        
        if (
            !(px == 0 && xDirs[i] == -1) &&
            px + xDirs[i] < 256 &&
            !(py == 0 && yDirs[i] == -1) &&
            py + yDirs[i] < 256
        ) {
            neighbours.emplace_back(
                frame.getPixel(
                    px + xDirs[i],
                    py + yDirs[i]
                )
            );
        }
    }
    
    return neighbours;
}

}

// Returns a list of the Blobs found in a given frame
std::vector<Blob> findBlobs(
    const Frame& frame,
    const unsigned int threshold
) noexcept {
    std::vector<Blob> blobList;
    // A map of boolean values as to whether the pixel has been added to a blob
    bool isBlobbed[256][256];
    for (unsigned int i = 0; i < 256; ++i) {
        for (unsigned int j = 0; j < 256; ++j) {
            isBlobbed[i][j] = false;
        }
    }
    
    // Iterate over the pixels in the frame
    for (const auto& p : frame.getPixels()) {
        // Check if the pixel has already been added to a blob
        // and that the pixel count value is above the threshold of 
        // consideration
        if (
            p.second.getC() >= threshold && 
            !isBlobbed[p.second.getX()][p.second.getY()]
        ) {
            // Add the pixel to a blob
            std::vector<std::uint32_t> addedKeys;
            addedKeys.emplace_back(p.first);
            isBlobbed[p.second.getX()][p.second.getY()] = true;
            
            // Iterate over pixels in the blob
            for (unsigned int i = 0; i < addedKeys.size(); ++i) {
                // Iterate over the neighbours of the pixel
                auto neighbours = getNeighbours(
                    frame,
                    frame.getPixel(addedKeys[i])
                );
                for (const auto& n : neighbours) {
                    // If the count value is above the threshold 
                    // and the neighbour hasn't been added to a blob
                    if (
                        n.getC() >= threshold && 
                        !isBlobbed[n.getX()][n.getY()]
                    ) {
                        // Add the pixel to the blob adn set as considered
                        addedKeys.emplace_back(n.getX() * 256 + n.getY());
                        isBlobbed[n.getX()][n.getY()] = true;
                    }
                }
            }
            
            // Add keys to a blob
            Blob blob;
            for (const auto k : addedKeys) {
                blob.addPixelKey(k);
            }
            // Add the blob to the list
            blobList.emplace_back(blob);
        }
    }

    return blobList;
}

} // lane
