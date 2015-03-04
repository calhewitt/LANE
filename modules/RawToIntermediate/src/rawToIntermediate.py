#!/usr/bin/env python2

import os, sys, struct, glob


def peek(f, size = 1):
    pos = f.tell()
    data = f.read(size)
    f.seek(pos)
    return data

class CompressionMode:
    RLE = 'RLE'
    XYV = 'XYV'
    NoCompression = 'No Compression'
    Unknown = 'Unknown'
        
class LUCIDFile:
    def __init__(self):
        self.clear()
    
    def clear(self):
        self.chipActive = [False, False, False, False, False]
        self.startTime = 0
        self.fileID = ''
        self.shutterRate = 0
        self.isCompressed = False
        self.isLinearLUT = False
        self.compressionMode = CompressionMode.Unknown
        
    def getIsChipActive(self, chipNumber):
        return self.chipActive[chipNumber]
        
    def setIsChipActive(self, chipNumber, value):
        self.chipActive[chipNumber] = value
        
    def getStartTime(self):
        return self.startTime
        
    def setStartTime(self, value):
        self.startTime = value
        
    def getFileID(self):
        return self.fileID
        
    def setFileID(self, value):
        self.fileID = value
        
    def getShutterRate(self):
        return self.shutterRate
        
    def setShutterRate(self, value):
        self.shutterRate = value
        
    def getIsCompressed(self):
        return self.isCompressed
        
    def setIsCompressed(self, value):
        self.isCompressed = value
        
    def getIsLinearLUT(self):
        return self.isLinearLUT
        
    def setIsLinearLUT(self, value):
        self.isLinearLUT = value
        
    def getCompressionMode(self):
        return self.compressionMode
        
    def setCompressionMode(self, value):
        self.compressionMode = value

    def convert(self, fileName, outputPath):
        # -Header Format:-
        # First 2 bytes are header magic - 0xDC 0xCC
        # 1 byte - Matrix table bits [1:0] and a bit field of the active
        # channel
        # 1 byte - Matrix table bits [9:2]
        # 2 bytes - Shutter mode
        # 1 byte - compression bitfield
        # 1 byte - shutter rate
        # 4 bytes - start time
        # 4 bytes - file ID

        # -Frame Format:-
        # Start of Frame demarker
        # Control data word w/ timestamp
        # and multiple of the following until the next time stamp:
        # Control data word w/ channel marker, 5 bit bitfield indexing from 0 at 2nd
        # byte
        # Data payload
        # Pixels are stored in 256 word rows from bottom left pixel to top right

        # -Word Format:-
        # 16-bit words, first 1/2 bits are flag bits, options are:
        # 0  - Number of zeroes
        # 10 - Payload data
        # 11 - Control data - Start of frame data characters etc.
        self.clear()

        input = open(fileName, 'rb')
        fileSize = os.path.getsize(fileName)

        output = open(outputPath + os.path.splitext(os.path.basename(fileName))[0] + '.lane', 'wb')

        # Check for valid header
        current = input.read(2)
        if (ord(current[0]) == 0xDC and ord(current[1]) == 0xCC):
            # Channel
            # The active detectors are flagged using a bit field.
            # This is only the last 5 bits of the byte.
            # 1F would translate to 00011111
            # This would mean all 5 detectors are taking data for this run.
            # I do not know which way round the bit field is
            # so whether TPX0 is bit 3 or bit 7 is unknown as of yet
            current = input.read(1)
            for i in range(0, 5):
                self.chipActive[i] = True if ((ord(current) >> i) & 0x01) == 0x01 else False;

            current = input.read(3) # skip 3 bytes

            # Compression
            # Type of compression
            # 0 - RLE
            # 1 - XYV
            current = input.read(1)
            temp = ((ord(current) >> 2) & 0x01)
            if temp == 0x00:
                self.compressionMode = CompressionMode.RLE
            elif temp == 0x01:
                self.compressionMode = CompressionMode.XYV
            else:
                self.compressionMode = CompressionMode.Unknown
            # Type of LUT used
            # 0 - PRN
            # 1 - Linear
            self.isLinearLUT = True if ((ord(current) >> 1) & 0x01) == 0x01 else False;
            # Data compression enabled?
            # 0 - Off
            # 1 - On
            self.isCompressed = True if (ord(current) & 0x01) == 0x01 else False;
            if not self.isCompressed:
                self.compressionMode = CompressionMode.NoCompression

            # Shutter Rate
            current = input.read(1)
            self.shutterRate = current

            # Start Time
            current = input.read(4)
            self.startTime = struct.unpack('>I', current[0:4])[0]

            # File ID
            current = input.read(4)
            for i in range(0, 4):
                self.fileID += chr(ord(current[i]))
                self.fileID = self.fileID[::-1]

            print 'Found valid header!'
                
        else:
            print 'Unable to find a valid header, assuming defaults...'
            self.compressionMode = CompressionMode.Unknown
            self.isCompressed = False
            self.isLinearLUT = False
            self.startTime = 0
            self.fileID = '????'
            for i in range(0, 5):
                self.chipActive[i] = True # Assume all chips are active
            input.seek(14)

        print 'Compression mode: ', self.compressionMode
        print 'Compressed? ', 'True' if (self.isCompressed) else 'False'
        print 'Linear LUT? ', 'True' if (self.isLinearLUT) else 'False'
        print 'Start Time: ', self.startTime
        print 'Config ID: ', self.fileID
        print 'Active Chips: '
        for i in range(0, 5):
            print 'Chip ', i, ' is ', 'active' if (self.chipActive[i]) else 'inactive'

        output.write(self.fileID + ',' + str(self.startTime) + '\n')

        # Parse main body
        # Read in frames
        isFindingFrames = True
        while isFindingFrames:
            # Check for frames demarkers
            current = input.read(2)
            if not (ord(current[0]) == 0xDC and ord(current[1]) == 0xDF):
                raise Exception('Expected frame header, Position = ' + str(input.tell()))

            # Get timestamp - 4 byte UNIX time, 1 byte sub second time
            current = input.read(4)
            frameTime = int(struct.unpack('>I', current)[0])
            current = input.read(1)
            frameTimeSub = int(ord(current))
            
            isFindingChannelData = True
            while isFindingChannelData:
                # Get channel
                current = input.read(1)
                channel = 5
                if (ord(current) >> 6) == 0x03:
                    if (ord(current) & 0x01) == 0x01:
                        channel = 0
                    elif ((ord(current) & 0x02) >> 1) == 0x01:
                        channel = 1
                    elif ((ord(current) & 0x04) >> 2) == 0x01:
                        channel = 2
                    elif ((ord(current) & 0x08) >> 3) == 0x01:
                        channel = 3
                    elif ((ord(current) & 0x16) >> 4) == 0x01:
                        channel = 4
                else:
                    raise Exception("Expected channel control word, Position = " + str(input.tell()))

                output.write(str(channel) + '\n')
                output.write(str(frameTime) + '.' + str(frameTimeSub) + '\n')

                # Xs and Ys starting from the bottom left of the pixel grid
                x = 0
                y = 0
                # Get data until next control word (channel marker or frame demarking)
                isFindingData = True
                while isFindingData:
                    # Check if this is the last word, and stop reading after this iteration if so
                    currentPos = input.tell()
                    current = peek(input, 2)
                    if currentPos + 2 >= fileSize:
                        isFindingData = False;
                        isFindingChannelData = False;
                        isFindingFrames = False;
                    # Check the next word for a new frame demarker
                    elif ord(current[0]) == 0xDC and ord(current[1]) == 0xDF:
                        isFindingData = False
                        isFindingChannelData = False
                    # Check for a channel marker
                    elif (ord(current[0]) >> 6) == 0x03:
                        isFindingData = False

                    # Get actual data
                    current = peek(input, 1)
                    if (ord(current) >> 6) == 0x02:
                        # Payload data (10-)
                        current = input.read(2)
                        payload = int(struct.unpack('>H', current[0:2])[0])
                        payload = payload & 32767;
                        
                        if payload > 0:
                            output.write(str(x) + ',' + str(y) + ',' + str(payload) + '\n')
                        
                        if x == 255:
                            x = 0
                            y += 1
                        else:
                            x += 1

                    elif (ord(current) >> 7) == 0x00:
                        # Number of zeroes (0-)
                        current = input.read(2)
                        numberOfZeroes = ord(struct.unpack('>H', current[0:2])[0])
                        for i in range(0, numberOfZeroes):
                            currentFrame.setPixel(pixel)
                            output.write(str(x) + ',' + str(y) + ',' + str(payload) + '\n')
                            if x == 255:
                                x = 0
                                y += 1
                            else:
                                x += 1

                output.write('EOF\n')
            output.write('EOC\n')


def lucidFileToLane(input, outputPath):
    lf = LUCIDFile()
    lf.convert(input, outputPath)
    return

    
def listFiles(directory, extension = ''):
    """Gets a list of files from a given directory of a given extension

    :directory: The directory to search
    :extension: The extension of the files to find. If extension is left empty,
    grabs every file in a directory
    :returns: A list of absolute file path strings

    """
    absDirectory = os.path.abspath(directory)
    if extension == '':
        globString = os.path.join(absDirectory, '*')
    else:
        globString = os.path.join(absDirectory, '*.' + extension)
    fileList = []

    for f in glob.glob(globString):
        fileList.append(f)

    return fileList

    
if __name__ == '__main__':
    if len(sys.argv) != 6:
        print "USAGE: %s input-dir output-dir masks-dir calibrations-dir configurations-dir" % (str(sys.argv[0]))
        sys.exit(1)
    
    fileTypes = {'ldat':lucidFileToLane}

    for ext, converter in fileTypes.iteritems():
        print 'Converting', ext, 'files...'
        for p in listFiles(sys.argv[1], ext):
            isConverted = False
            for q in listFiles(sys.argv[2], 'lane'):
                if os.path.splitext(os.path.basename(p))[0] == os.path.splitext(os.path.basename(q))[0]:
                    isConverted = True
            if not isConverted:
                print p
                print "Converting ", p
                converter(p, sys.argv[2])
