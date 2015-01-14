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
#include "LANEFile.hpp"
#include "Pixel.hpp"


class Cluster final {
public:
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Cluster();
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ~Cluster() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    /// \param other The other Cluster object to copy construct from
    Cluster(const Cluster& other);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    /// \param other The other Cluster object to move construct from
    Cluster(Cluster&& other);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Assignment operator
    /// \param other The other Cluster object to assign from
    Cluster& operator=(const Cluster& other);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assignment operator
    /// \param other The other Cluster object to move assign from
    Cluster& operator=(Cluster&& other);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Equality operator
    /// \param other The other Cluster object to equate against
    bool operator==(const Cluster& other) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Inequality operator
    /// \param other The other Cluster object to equate against
    bool operator!=(const Cluster& other) const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Clears the cluster of pixels
    void clear() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Adds a pixel to the cluster
    /// \param pixel The pixel object to add to the cluster
    void addPixel(const lane::Pixel& pixel) noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves a vector of the pixels currently in the cluster
    /// \return A vector of pixels in the cluster
    const std::vector<lane::Pixel>& getPixels() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the size of the cluster in pixels
    /// \return The size of the cluster (number of pixels in the cluster)
    unsigned int getSize() const noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the cluster volume
    /// \return The total energy deposited in the cluster (volume)
    double getVolume() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the cluster height
    /// \return The energy deposited in the most energetic pixel (height)
    double getHeight() const noexcept;
    
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
    double getAzimuthAngle() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the projected track length of a particle based on
    /// its footprint at the projected plane of detector
    /// \return The projected track length
    double getProjectedTrackLength() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief The track length of particle from the point it enters to the 
    /// point it exits (assumes penetrating particle)
    /// \return The track length
    double getTrackLength() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the angle in radians with respect to the vertical
    /// direction
    /// \return The polar angle
    double getPolarAngle() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Retrieves the LET in Si
    /// \return The LET in Si
    double getLETinSi() noexcept;
    
    //////////////////////////////////////////////////////////////////////////
    /// \brief Calculates the area in pixels which the particle could have hit
    /// without it touching the edge of the frame
    /// \return Area in pixels
    unsigned int getHittingArea() const noexcept;

    //////////////////////////////////////////////////////////////////////////
    /// \brief Returns a boolean representing wether or not it is touching the edge of the frame
    /// \return Boolean representing wether or not it is touching the edge
    bool touchingEdge() const noexcept;
    
    //////////////////////////////////////////////////////////////////////////
    /// \brief Calculates X Bar
    /// \return The X Bar
    float  getXBar() noexcept;
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief Calculates Y Bar
    /// \return The Y Bar
    float getYBar() noexcept;

private:
    // Caclulates the fuzzy track length of the cluster on the x-axis
    // (or y-axis) from its profile and a cut point (coeff)
    double getFuzzyTrackLength(
        std::map<int, double>& energyHis,
        const double coeff
    ) noexcept;

    // The storage for cluster pixels
    std::vector<lane::Pixel> pixels_;
    
    // cluster parameters
    double volume_;
    double LET_;
    double height_;
    float biasVoltage_;
    int detectorThickness_;
    double azimuthAngle_;
    double polarAngle_;
    double majorLength_;
    double minorWidth_;
    double projectedTrackLength_;
    double trackLength_;
    unsigned int xmin_;
    unsigned int xmax_;
    unsigned int ymin_;
    unsigned int ymax_;
    float xbar_;
    float ybar_;
};

#endif // BASICCLUSTERANALYSIS_HPP
