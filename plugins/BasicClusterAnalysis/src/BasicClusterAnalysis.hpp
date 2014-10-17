///////////////////////////////////////////////////////////////////////////////
/// \file BasicClusterAnalysis.hpp
/// \author Hector Stalker <hstalker0@gmail.com> &
/// Sam Kittle. Based upon Son Hoang's code.
/// \version 0.1
///
/// \brief Class for performing basic cluster analysis on detector data
///
/// \copyright Copyright (c) 2014, Hector Stalker & Sam Kittle.
/// All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#ifndef BASICCLUSTERANALYSIS_HPP
#define BASICCLUSTERANALYSIS_HPP

#include <vector>
#include <map>
#include <cstdlib> 
#include <cmath>
#include "LUCID/LUCIDFile.hpp"
#include "Pixel.hpp"


class Cluster final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Cluster() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Cluster() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param x The x value
    Cluster(const Cluster& other) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param x The x value
    Cluster(Cluster&& other) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Assignment operator
    /// \param x The x value
    Cluster& operator=(const Cluster& other) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param x The x value
    Cluster& operator=(Cluster&& other) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param x The x value
    bool operator==(const Cluster& other) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param x The x value
    bool operator!=(const Cluster& other) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Clears the cluster of pixels
    void clear() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds a pixel to the cluster
    /// \param pixel The pixel object to add to the cluster
    void addPixel(const lucid::Pixel& pixel) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves a vector of the pixels currently in the cluster
    /// \return A vector of pixels in the cluster
    const std::vector<lucid::Pixel>& getPixels() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the size of the cluster in pixels
    /// \return The size of the cluster (number of pixels in the cluster)
    unsigned int getSize() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief ***
    /// \return The ***
    float getVolume() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief ***
    /// \return The ***
    float getHeight() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the bias voltage of the chip
    /// \param biasVoltage The bias voltage of the chip
    void setBiasVoltage(const float biasVoltage) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Sets the thickness of the detector in use
    /// \param detectorThickness The thickness of the detector
    void setDetectorThickness(const int detectorThickness) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the azimuth angle in radians - the angle between the
    /// projected vector and the x-coordinate vector on the projected plane
    /// of detector.
    /// \return The azimuth angle
    float getAzimuthAngle() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the projected track length of a particle based on
    /// its footprint at the projected plane of detector
    /// \return The projected track length
    float getProjectedTrackLength() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief The track length of particle from the point it enters to the 
    /// point it exits (assumes penetrating particle)
    /// \return The track length
    float getTrackLength() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the angle in radians with respect to the vertical
    /// direction
    /// \return The polar angle
    float getPolarAngle() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the LET in Si
    /// \return The LET in Si
    float getLETinSi() noexcept;

private:
    // Caclulates the fuzzy track length of the cluster on the x-axis
    // (or y-axis) from its profile and a cut point (coeff)
    float getFuzzyTrackLength(
        std::map<int, double>& energyHis,
        const float coeff
    ) noexcept;

    // The storage for cluster pixels
    std::vector<lucid::Pixel> pixels_;
    
    // cluster parameters
    float volume_;
    float LET_;
    float height_;
    float biasVoltage_;
    int detectorThickness_;
    float azimuthAngle_;
    float polarAngle_;
    float majorLength_;
    float minorWidth_;
    float projectedTrackLength_;
    float trackLength_;
};


#endif // BASICCLUSTERANALYSIS_HPP
