///////////////////////////////////////////////////////////////////////////////
/// \file Main.cpp
/// \author Hector Stalker <hstalker0@gmail.com>
/// \version 0.1
///
/// \brief A tool for splitting the LUCID data files up into smaller ones
///
/// \copyright Copyright (c) 2014, Hector Stalker. All rights reserved.
/// This file is under the Simplified (2-clause) BSD license
/// For conditions of distribution and use, see:
/// http://opensource.org/licenses/BSD-2-Clause
/// or read the 'LICENSE.md' file distributed with this code

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>


// Writes a vector of bytes to the file
void writeBlock(std::ofstream& output, std::vector<std::uint8_t> data) {
    if (!output.is_open()) {
        throw std::runtime_error("Tried to write to an unopen file!");
    }

    output.write(reinterpret_cast<char*>(&data[0]), data.size());
    output.flush();
}


// Splits the file into more frames
void splitFile(const std::string& filePath, const unsigned int numberOfFrames) {
    const unsigned int headerSize = 16;

    std::ifstream input(filePath, std::ios::binary | std::ios::in | std::ios::ate);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    // Get file size
    const std::uint64_t fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    // Read header into heap array
    std::vector<std::uint8_t> fileHeader;
    fileHeader.resize(16);
    input.read(reinterpret_cast<char*>(&fileHeader[0]), headerSize);

    // Integrity checks
    // Check the file magic
    if (fileHeader[0] != 0xDC && fileHeader[1] != 0xCC) {
        throw std::runtime_error(
            "'" + filePath + "' is not a LUCID data file"
        );
    }
    // Check that the file size is big enough for the header
    if (fileSize < headerSize) {
        throw std::runtime_error(
            "'" + filePath + "' is not a valid LUCID data file"
        );
    }

    // Read in frames
    bool isFindingFrames = true;
    std::uint64_t fileRemaining = fileSize - headerSize;
    std::uint64_t index = 0;
    std::uint64_t currentFrameCount = 0;
    unsigned int currentFileNumber = 1;
    std::vector<std::uint8_t> fileData;
    std::ofstream currentOutput;
    while (isFindingFrames) {
        if (currentFrameCount == 0) {
            currentOutput.open(
                filePath + "_" + std::to_string(currentFileNumber),
                std::ios::binary | std::ios::out
            );
            writeBlock(currentOutput, fileHeader);
        }

        // Check integrity for demarker and timestamp
        if (index + 7 > fileRemaining) {
            throw std::runtime_error("Malformed data file - Unexpected end of file in frame");
        }

        fileData.resize(fileData.size() + 7);
        input.read(reinterpret_cast<char*>(&fileData[index]), 7);
        // Check for frames demarkers
        if (!(fileData[index] == 0xDC && fileData[index + 1] == 0xDF)) {
            throw std::runtime_error("Malformed data file - Expected frame header");
        }

        // Move forwards 7 bytes
        index += 7;

        bool isFindingChannelData = true;
        while (isFindingChannelData) {
            if (index + 1 > fileRemaining - 1) {
                throw std::runtime_error("Malformed data file - Unexpected end of file in channel header");
            }

            fileData.resize(fileData.size() + 1);
            input.read(reinterpret_cast<char*>(&fileData[index]), 1);
            // Get channel
            if ((fileData[index] >> 6) != 0x03) {
                throw std::runtime_error("Malformed data file - Expected channel control word");
            }

            // Move past the channel marker
            index += 1;

            // Get data until next control word (channel marker or frame demarking)
            bool isFindingData = true;
            while (isFindingData) {
                // Get actual data
                fileData.resize(fileData.size() + 2);
                input.read(reinterpret_cast<char*>(&fileData[index]), 2);
                index += 2;
                // Check if this is the last word, and stop reading after this iteration if so
                if (index > fileRemaining - 1) {
                    isFindingData = false;
                    isFindingChannelData = false;
                    isFindingFrames = false;
                // Check the next word for a new frame demarker
                } else if (input.get() == 0xDC) {
                    if (input.get() == 0xDF) {
                        isFindingData = false;
                        isFindingChannelData = false;
                    }
                    input.unget();
                    input.unget();
                // Check for a channel marker
                } else {
                    input.unget();
                    if ((input.get() >> 6) == 0x03) {
                        isFindingData = false;
                    }
                    input.unget();
                }
            }
        }

        // Increment the frame count and check to see if the block of data needs to be written
        currentFrameCount += 1;

        if (currentFrameCount == numberOfFrames) {
            currentFrameCount = 0;
            writeBlock(currentOutput, fileData);
            fileRemaining -= fileData.size();
            fileData.clear();
            index = 0;
            currentFileNumber += 1;
            currentOutput.close();
        }
    }

    // If the end of the file is reached before reaching the frame cap
    // Write the stored up data out
    if (fileData.size() > 0) {
        writeBlock(currentOutput, fileData);
        currentOutput.close();
    }
}


int main(int argc, char* argv[]) {
    unsigned int frameCount = 4000;

    if (argc < 2 || argc > 3) {
        std::cout << "USAGE: " << argv[0] << " inputPath [numberOfFramesPerFile]\n"
            << "Where [] implies the parameter is optional. Number of frames per file defaults to "
            << frameCount << " frames" << "\n";
        return 0;
    }

    try {
        if (argc == 3) {
            frameCount = std::stoi(std::string(argv[2]));
        }

        splitFile(argv[1], frameCount);
    } catch (const std::runtime_error& e) {
        std::cout << "An error occurred: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cout << "An error occurred: " << e.what() << "\n";
        return 2;
    } catch (...) {
        std::cout << "An unknown exception was thrown\n";
        return 3;
    }

    return 0;
}

