///////////////////////////////////////////////////////////////////////////////
/// \file Main.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief Module for converting raw data files to a common data format
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
#include <map>
#include <stdexcept>
#include "Utils/Filesystem.hpp"
#include "RawInputFile.hpp"
#include "LUCID/LUCIDFile.hpp"
#include "LANEFile.hpp"
#include "Utils/Misc.hpp"


int main(int argc, char *argv[]) {
    using namespace std;
    using namespace lane;
    using namespace lane::lucid;
    using namespace lane::utils;
    
    if (argc != 3) {
        cout << "USAGE: " << argv[0] << " input-directory output-directory\n";
        return 1;
    }
    
    string inputPath = argv[1];
    string outputPath = argv[2];
    
    // Extension to file parser map
    map<string, shared_ptr<RawInputFile>> inputParsers = {
        { "ldat", make_shared<LUCIDFile>() }, // LUCID
    };
    
    // Get the list of input file paths
    for (auto& p : inputParsers) {
        cout << "Converting " << p.first << " files\n";
        auto inputs = getFilesWithExtension(p.first, inputPath);
        // Iterate over the input files
        for (const auto& input : inputs) {
            try {
                cout << "File - " << input << "\n";
                p.second->read(input);
                
                LANEFile out;
                out.setFileID(p.second->getFileID());
                out.setStartTime(p.second->getStartTime());
                // Add frames to the LANE intermediate object
                for (const auto& x : p.second->getChannelToFramesMap()) {
                    for (const auto& y : x.second) {
                        out.addFrame(y, x.first);
                    }
                }
                
                // Write the LANE intermediate out
                out.write(removeExtension(input) + ".lane");
            } catch (const std::runtime_error& e) {
                cout << "An error occurred.\n" << e.what() << "\n";
            } catch (...) {
                cout << "An unknown error occurred...\n";
            }
        }
    }
    
	return 0;
}
