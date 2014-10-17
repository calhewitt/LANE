///////////////////////////////////////////////////////////////////////////////
/// \file BasicClusterAnalysis.cpp
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

#include <list>
#include <map>
#include <cstdlib>
#include <cmath>
#include "Pixel.hpp"
#include "BasicClusterAnalysis.hpp"

// TODO: Add support for getting min x/y and max x/y form clusters

const double pi = 3.14159265359;

namespace {

// Rotates the pixel's position using the azimuth angle
lucid::Pixel rotatePixel(const lucid::Pixel& pixel, const float azimuthAngle) noexcept {
    lucid::Pixel rotatedPixel;
    
    rotatedPixel.setX(pixel.getX()); // TODO Implement rotations
    rotatedPixel.setY(pixel.getY());
    rotatedPixel.setC(pixel.getC());
    
    return rotatedPixel;
}

}

Cluster::Cluster() noexcept
: volume_(0),
  height_(0),
  azimuthAngle_(-pi),
  polarAngle_(-1),
  majorLength_(-1),
  minorWidth_(-1),
  projectedTrackLength_(-1),
  trackLength_(0) {
}

Cluster::~Cluster() noexcept {
    clear();
    volume_ = 0;
    height_ = 0;
    azimuthAngle_ = -pi;
    polarAngle_ = -1;
    majorLength_ = -1;
    minorWidth_ = -1;
    projectedTrackLength_ = -1;
    trackLength_ = 0;
}

Cluster::Cluster(const Cluster& other) noexcept = default;

Cluster::Cluster(Cluster&& other) noexcept = default;

Cluster& Cluster::operator=(const Cluster& other) noexcept = default;

Cluster& Cluster::operator=(Cluster&& other) noexcept = default;

bool Cluster::operator==(const Cluster& other) const noexcept {
    if (this != &other) {
        return (
            pixels_ == other.pixels_ &&
            volume_ == other.volume_ &&
            LET_ == other.LET_ &&
            height_ == other.height_ &&
            biasVoltage_ == other.biasVoltage_ &&
            detectorThickness_ == other.detectorThickness_ &&
            azimuthAngle_ == other.azimuthAngle_ &&
            polarAngle_ == other.polarAngle_ &&
            majorLength_ == other.majorLength_ &&
            minorWidth_ == other.minorWidth_ &&
            projectedTrackLength_ == other.projectedTrackLength_ &&
            trackLength_ == other.trackLength_
        );
    }

    return true;
}

bool Cluster::operator!=(const Cluster& other) const noexcept {
    return !(*this == other);
}

void Cluster::clear() noexcept {
    pixels_.clear();
}

void Cluster::addPixel(const lucid::Pixel& pixel) noexcept {
    pixels_.emplace_back(pixel);
    volume_ += pixel.getC();
    if (pixel.getC() > height_) {
        height_ = pixel.getC();
    }
}

const std::vector<lucid::Pixel>& Cluster::getPixels() const noexcept {
    return pixels_;
}

unsigned int Cluster::getSize() const noexcept {
    return pixels_.size();
}

float Cluster::getVolume() const noexcept {
    return volume_;
}

float Cluster::getHeight() const noexcept {
    return height_;
}

void Cluster::setBiasVoltage(const float biasVoltage) noexcept {
    biasVoltage_ = biasVoltage;
}

void Cluster::setDetectorThickness(const int detectorThickness) noexcept {
    detectorThickness_ = detectorThickness;
}

float Cluster::getAzimuthAngle() noexcept {
    if (azimuthAngle_ > -pi) {
        return azimuthAngle_;
    }
    if (getSize() == 1) {
        return 0;
    }
    
    float threshold = height_ / 10;
    float wx, wy, wxx, wyy; // sum of weighted x, y, x*x, y*y
    float wxy; // sum of weighted x*y
    float fw; // sum of weight
    unsigned int x, y; // coordinate x, y at each pixel
    
    wx = wy = wxx = wxy = wyy = 0;
    fw = 0;

    for (auto iter = std::begin(pixels_); iter != std::end(pixels_); ++iter) {
        if (iter->getC() > threshold) {
            float w; // energy of each pixel
             x = iter->getX();
             y = iter->getY();
             w = iter->getC();
             wx += x * w;
             wy += y * w;
             wxx += x * x * w;
             wxy += x * y * w;
             fw += w;
			 wyy += w * y * y;
        }
    }
    float delta;
    delta = fw * wxy - wy * wx;

    if (std::abs(delta) <= 0) {
        azimuthAngle_ = 0;
    } else {
		//Equation for line which minimizes the square of the distance to the line
		//Can be 90 degrees out as the maths produces a quadratic with two roots. Works out that m1 = -1/m2 so they are 90degrees apart
		//The program gives one root and then when calculating polar it does if minWidth > majLen then change azimuth by pi/20
		//It then does another histogram for majLen using c ** 2 for each pixel and looks at the mean for this
		//It compares the mean value for x weighted by c**2 to the mean value weighted by c
		float a = fw * wxx - wx * wx;
		float b = 2 * (wxy * fw - wx * wy);
		float c = fw * wyy - wy * wy;
        azimuthAngle_ = std::atan((c - a + std::sqrt((a-c)*(a-c) - b * b))/b);
	}
    return azimuthAngle_;
}

float Cluster::getProjectedTrackLength() noexcept {
    if (projectedTrackLength_ >= 0) {
        return projectedTrackLength_;
    }

    if (getSize() <= 2) {
        majorLength_ = 0;
        minorWidth_ = 0;
        projectedTrackLength_ = 0;
        return projectedTrackLength_;
    }

    float threshold = height_ / 20;

    float azimuth = getAzimuthAngle();
    std::map<int, double> majorHis, minorHis;
    std::map<int, double>::iterator p;
	std::map<int, double> majorHiscc, minorHiscc;
    //the projected track is assumed to be parallel to X-axis if 0< |azimuthAngle| < 10 degree and parallel to Y-axis if 80 < |azimuthAngle| < 90 degree

	//need to rotate the coordinate
	std::list<double> rotatedData;//list of triple<x,y,v>
	for (auto iter = std::begin(pixels_); iter != std::end(pixels_); ++iter) {
		if (iter->getC() > threshold) {
            auto newPixel = rotatePixel(*iter, -azimuth);
            rotatedData.emplace_back(newPixel.getX());
            rotatedData.emplace_back(newPixel.getY());
            rotatedData.emplace_back(newPixel.getC());
		}
	}
	for (auto iter = std::begin(rotatedData); iter != std::end(rotatedData); ++iter) {
		int majorBin = (int)(*iter); ++iter;
		int minorBin = (int)(*iter); ++iter;
		double v = *iter;
		//accumulate energy on rotated X to set up a major Histogram
		p = majorHis.find(majorBin);
		if (p == std::end(majorHis)) {
			majorHis[majorBin] = v;
        } else {
			p->second += v;
        }

		//accumulate energy on rotated Y to set up a minor Histogram
		p = minorHis.find(minorBin);
		if (p == std::end(minorHis)) {
			minorHis[minorBin] = v;
        } else {
			p->second += v;
        }
	}
	
    for (auto iter = std::begin(rotatedData); iter != std::end(rotatedData); ++iter) {
		int majorBin = (int)(*iter); ++iter;
		int minorBin = (int)(*iter); ++iter;
		double v = *iter;
		//accumulate energy on rotated X to set up a major Histogram
		p = majorHiscc.find(majorBin);
		if (p == std::end(majorHis)) {
			majorHiscc[majorBin] = v*v;
        } else {
			p->second += v*v;
        }

		//accumulate energy on rotated Y to set up a minor Histogram
		p = minorHiscc.find(minorBin);
		if (p == std::end(minorHiscc)) {
			minorHiscc[minorBin] = v*v;
        } else {
			p->second += v*v;
        }
	}

    if (majorHis.size() <= 2 && minorHis.size() <= 2) {
		//To small to work anything out from it
        majorLength_ = 0;
        minorWidth_ = 0;
        projectedTrackLength_ = 0;
        return projectedTrackLength_;
    }

    majorLength_ = getFuzzyTrackLength(majorHis, 0.75);
    minorWidth_ = getFuzzyTrackLength(minorHis, 0.75);
    if (majorLength_ < minorWidth_) {
		float temp = majorLength_;
        majorLength_ = minorWidth_;
		minorWidth_ = temp;
		azimuthAngle_ += pi/2;
    }
    float alpha = minorWidth_ / (2 * majorLength_ - minorWidth_);
    float coeff = 55;
    float correction = biasVoltage_ / 50.0;
    if (correction > 2) {
        correction = 2;
    }
    if (detectorThickness_ == 500) {
        correction += 5;
    } else if (detectorThickness_ == 1000) {
        correction += 10;
    }
    coeff += correction;
    projectedTrackLength_ = coeff * std::abs(majorLength_ - alpha * minorWidth_);
    return projectedTrackLength_;
}

float Cluster::getTrackLength() noexcept {
    if(trackLength_ > 0) {
        return trackLength_;
    }
    trackLength_ = std::sqrt(getProjectedTrackLength() * getProjectedTrackLength() + detectorThickness_ * detectorThickness_);
    return trackLength_;
}

float Cluster::getPolarAngle() noexcept {
    if (polarAngle_ >= 0) {
        return polarAngle_;
    }
    polarAngle_ = std::atan(getProjectedTrackLength() / detectorThickness_);
    return polarAngle_;
}

float Cluster::getLETinSi() noexcept {
    return getVolume() / getTrackLength();
}

float Cluster::getFuzzyTrackLength(
    std::map<int, double>& his,
    const float coeff
) noexcept {
    std::map<int, double>::iterator iter = std::begin(his);
    double arv = 0.0;
    float result = 0.0;
    for (; iter != std::end(his); ++iter) {
        arv += iter->second;
    }
    arv /= his.size();
    arv *= coeff;
    //assign the membership function for each bin, add up bins to get fuzzy track_length
    for (iter = std::begin(his); iter != std::end(his); ++iter) {
        if (iter->second > arv) {
            ++result;
        } else {
            result += iter->second / arv;
        }
    }
    return result;
}
