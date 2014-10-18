///////////////////////////////////////////////////////////////////////////////
/// \file Main.cpp
/// \author Hector Stalker <hstalker0@gmail.com> &
/// Sam Kittle
/// \version 0.1
///
/// \brief Main driver code for basic cluster analysis module
///
/// \copyright Copyright (c) 2014, Hector Stalker & Sam Kittle.
/// All rights reserved. 
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "Utils/Filesystem.hpp"
#include "Blob.hpp"
#include "BlobFinder.hpp"
#include "Frame.hpp"
#include "BasicClusterAnalysis.hpp"


int main(int argc, char *argv[]) {
    using namespace std;
    using namespace lucid;
    using namespace utils;
    
    if (argc != 3) {
        cout << "USAGE: " << argv[0] << " input-directory output-directory\n";
        return 1;
    }
    //TODO Load a, b, cand t calibration matrices here.

    string inputPath = argv[1];
    string outputPath = argv[2];
    
    // Get the list of input file paths
    auto inputs = getFilesWithExtension("ldat", inputPath);
    // Iterate over the input files
    for (const auto& input : inputs) {
        cout << "Running BCA on '" << input << "'";
        auto file = LUCIDFile(input);
        ofstream outf;
        vector<Cluster> clusters;
        
        outf.open(
            outputPath + "/BCA_" + removeExtension(getFileName(input)) + ".txt",
            fstream::out | fstream::binary
        );
        
        for (unsigned int channel = 0; channel < 5; ++channel) {
            cout << ".";
            outf << "Channel " << channel << "\n";
            unsigned int frameNumber = 1;
            for (const auto& f : file.getFrames(channel)) {
                for (const auto& b : findBlobs(f)) {
                    Cluster cl;
                    // Iterate over the keys in the blob
                    // TODO Set the appropriate bias voltage at some point
                    for (const auto k : b) {
                        Pixel p = f.getPixel(k);
                        // If it isn't calibrated (or I haven't implemented loading the values) 
                        // then use "typical" values
                        unsigned int x = p.getC();
                        // Typical (ish) values from the calibration
                        float a = 2, b = 80, c = 250, t = -0.1; 
                        // TODO load actual values of a, b, c and t
                        if (x == 0) {
                            p.setE(0);
                        } else {
                            p.setE(a * x + b + c / (x + t));
                            cl.addPixel(p);
                        }
                    }
                    
                    // Output the data in a simple way for now
                    outf << "Frame " << frameNumber << "\n";
                    outf << "TimeStamp " << f.getTimeStamp() << "." << f.getTimeStampSub() << "\n";
                    outf << "Azimuth " << cl.getAzimuthAngle() << "\n";
                    outf << "Polar " << cl.getPolarAngle() << "\n";
                    outf << "Volume " << cl.getVolume() << "\n";
                    outf << "Height " << cl.getHeight() << "\n";
                    outf << "HittingArea " << cl.getHittingArea() << "\n";
                    outf << "TouchingEdge " << cl.touchingEdge() << "\n";
                    outf << "LET " << cl.getLETinSi() << "\n\n\n";
                }
                
                ++frameNumber;
            }
        }
        cout << "\n";
    }
    
	return 0;
}
