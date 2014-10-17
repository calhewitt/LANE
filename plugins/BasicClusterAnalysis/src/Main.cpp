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
                    Cluster c;
                    // TODO: Do we need to set the detector thickness and volume?
                    // TODO: Figure out the units for the thickness etc.
                    // Iterate over the keys in the blob
                    for (const auto k : b) {
                        c.addPixel(f.getPixel(k));
                    }
                    
                    // Output the data in a simple way for now
                    outf << "Frame " << frameNumber << "\n";
                    outf << "ShutterTime " << f.getTimeStamp() << "." << f.getTimeStampSub() << "\n";
                    outf << "Azimuth " << c.getAzimuthAngle() << "\n";
                    outf << "Polar " << c.getPolarAngle() << "\n";
                    outf << "LET " << c.getLETinSi() << "\n\n\n";
                }
                
                ++frameNumber;
            }
        }
        cout << "\n";
    }
    
	return 0;
}
