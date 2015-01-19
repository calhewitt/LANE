#!/usr/bin/env python2

import os, sys, struct, glob


class CompressionMode:
    RLE = 'RLE'
    XYV = 'XYV'
    NoCompression = 'No Compression'
    Unknown = 'Unknown'

    
class Pixel:
    def __init__(self):
        self.clear()
    
    def clear(self):
        self.x = 0
        self.y = 0
        self.c = 0
    
    def getX(self):
        return self.x
        
    def setX(self, value):
        self.x = value
        
    def getY(self):
        return self.y
        
    def setY(self, value):
        self.y = value
        
    def getC(self):
        return self.c
        
    def setC(self, value):
        self.c = value
    
    def dump(self):
        print 'x = %d, y = %d, c = %d' % (self.x, self.y, self.c)


class Frame:
    def __init__(self):
        self.clear()
    
    def clear(self):
        self.channelID = 0
        self.timeStamp = 0
        self.timeStampSub = 0
        self.pixels = {}
    
    def getPixel(self, x, y):
        return self.pixels[x * 256 + y] 
        
    def setPixel(self, pixel):
        self.pixels[pixel.getX() * 256 + pixel.getY()] = pixel
        
    def getTimeStamp(self):
        return self.timeStamp
        
    def setTimeStamp(self, value):
        self.timeStamp = value
        
    def getTimeStampSub(self):
        return self.timeStampSub
        
    def setTimeStampSub(self, value):
        self.timeStampSub = value
    
    def getChannelID(self):
        return self.channelID
        
    def setChannelID(self, value):
        self.channelID = value
    
    def dump(self):
        print 'time stamp = %d.%d, channel ID = %d' % (self.timeStamp, self.timeStampSub, self.channelID)
        for k, v in self.pixels.iteritems():
            v.dump()

        
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
        self.channels = {0:[], 1:[], 2:[], 3:[], 4:[]}
        
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
        
    def getChannelData(self):
        return self.channels

    def read(self, fileName):
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

        with open(fileName, 'rb') as input:
            input.seek(0, os.SEEK_END)
            
            # Get file size
            fileSize = input.tell()
            input.seek(0)

            # Read into array
            data = input.read(fileSize)

        # Integrity checks
        # Check the file magic
        if ord(data[0]) != 0xDC and ord(data[1]) != 0xCC:
            raise Exception("'" + fileName + "' is not a LUCID data file")
        # Check that the file size is big enough for the header
        if fileSize < 16:
            raise Exception("'" + fileName + "' is not a valid LUCID data file")

        # Channel
        # The active detectors are flagged using a bit field.
        # This is only the last 5 bits of the byte.
        # 1F would translate to 00011111
        # This would mean all 5 detectors are taking data for this run.
        # I do not know which way round the bit field is
        # so whether TPX0 is bit 3 or bit 7 is unknown as of yet
        for i in range(0, 4):
            self.chipActive[i] = True if ((ord(data[2]) >> i) & 0x01) == 0x01 else False;

        # Compression
        # Type of compression
        # 0 - RLE
        # 1 - XYV
        temp = ((ord(data[6]) >> 2) & 0x01)
        if temp == 0x00:
            self.compressionMode = CompressionMode.RLE
        elif temp == 0x01:
            self.compressionMode = CompressionMode.XYV
        else:
            self.compressionMode = CompressionMode.Unknown
        # Type of LUT used
        # 0 - PRN
        # 1 - Linear
        self.isLinearLUT = True if ((ord(data[6]) >> 1) & 0x01) == 0x01 else False;
        # Data compression enabled?
        # 0 - Off
        # 1 - On
        self.isCompressed = True if (ord(data[6]) & 0x01) == 0x01 else False;
        if not self.isCompressed:
            self.compressionMode = CompressionMode.NoCompression

        # Shutter Rate
        self.shutterRate = data[7]

        # Start Time
        self.startTime = struct.unpack('>I', data[8:12])[0]

        # File ID
        for i in range(0, 4):
            self.fileID += chr(ord(data[12 + i]))

        # Read in frames
        isFindingFrames = True
        index = 16
        while isFindingFrames:
            # Check integrity for demarker and timestamp
            if index + 7 > fileSize:
                raise Exception('Malformed data file - Unexpected end of file in frame')

            # Check for frames demarkers
            if not (ord(data[index]) == 0xDC and ord(data[index + 1]) == 0xDF):
                raise Exception('Malformed data file - Expected frame header')

            # Get timestamp - 4 byte UNIX time, 1 byte sub second time
            frameTime = int(struct.unpack('>I', data[index + 2:index + 6])[0])
            frameTimeSub = int(ord(data[index + 6]))

            # Move forwards 7 bytes
            index += 7

            isFindingChannelData = True
            while isFindingChannelData:
                if index + 1 > fileSize - 1:
                    raise Exception('Malformed data file - Unexpected end of file in channel header')

                # Get channel
                channel = 5
                if (ord(data[index]) >> 6) == 0x03:
                    if (ord(data[index]) & 0x01) == 0x01:
                        channel = 0
                    elif ((ord(data[index]) & 0x02) >> 1) == 0x01:
                        channel = 1
                    elif ((ord(data[index]) & 0x04) >> 2) == 0x01:
                        channel = 2
                    elif ((ord(data[index]) & 0x08) >> 3) == 0x01:
                        channel = 3
                    elif ((ord(data[index]) & 0x16) >> 4) == 0x01:
                        channel = 4
                else:
                    raise Exception("Malformed data file - Expected channel control word")

                # Move past the channel marker
                index += 1

                # Xs and Ys starting from the bottom left of the pixel grid
                x = 0
                y = 0
                # Create the frame
                currentFrame = Frame()
                currentFrame.setTimeStamp(frameTime);
                currentFrame.setTimeStampSub(frameTimeSub);
                currentFrame.setChannelID(channel);
                # Get data until next control word (channel marker or frame demarking)
                isFindingData = True
                while isFindingData:
                    # Check if this is the last word, and stop reading after this iteration if so
                    if index + 2 >= fileSize - 1:
                        isFindingData = False;
                        isFindingChannelData = False;
                        isFindingFrames = False;
                    # Check the next word for a new frame demarker
                    elif ord(data[index + 2]) == 0xDC and ord(data[index + 3]) == 0xDF:
                        isFindingData = False
                        isFindingChannelData = False
                    # Check for a channel marker
                    elif (ord(data[index + 2]) >> 6) == 0x03:
                        isFindingData = False

                    # Get actual data
                    if (ord(data[index]) >> 6) == 0x02:
                        # Payload data (10-)
                        payload = int(struct.unpack('>H', data[index:index + 2])[0])
                        payload = payload & 32767;
                        if x == 255:
                            x = 0
                            y += 1
                        else:
                            x += 1

                        if True:#payload > 0:
                            pixel = Pixel()
                            pixel.setX(x)
                            pixel.setY(y)
                            pixel.setC(payload)
                            currentFrame.setPixel(pixel)
                    elif (ord(data[index]) >> 7) == 0x00:
                        # Number of zeroes (0-)
                        numberOfZeroes = ord(struct.unpack('>H', data[index:index + 3])[0])
                        for i in range(0, numberOfZeroes):
                            pixel = Pixel()
                            pixel.setX(x)
                            pixel.setY(y)
                            pixel.setC(0)
                            currentFrame.setPixel(pixel)
                            if x == 255:
                                x = 0
                                y += 1
                            else:
                                x += 1

                    index += 2;

                # Add frame to channel map
                self.channels[channel].append(currentFrame)


    def dump(self):
        #print 'file ID = %s, start time = %d, shutter rate = %d, is compressed = %s, compression mode = %s, is linear LUT = %s'\
        #    % (self.fileID, self.startTime, self.shutterRate, str(self.isCompressed), self.compressionMode, str(self.isLinearLUT))
        print 'file ID = %s, start time = %d'\
            % (self.fileID, self.startTime)
        for i in range(0, len(self.chipActive)):
            print 'chip %s = %s' % (str(i), str(self.chipActive[i]))
        for k, v in self.channels.iteritems():
            for frame in v:
                frame.dump()


def LUCIDFileToLane(input):
    lf = LUCIDFile()
    lf.read(input)
    out = open('./' + os.path.splitext(os.path.basename(input))[0] + '.ldat', 'wb')
    lf.dump()
    out.close()
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
    if len(sys.argv) != 3:
        print "USAGE: %s input-path output-path" % (str(sys.argv[0]))
        sys.exit(1)
    
    fileTypes = {'ldat':LUCIDFileToLane}

    for ext, converter in fileTypes.iteritems():
        #print 'Converting', ext, 'files...'
        #for p in listFiles(sys.argv[1], ext):
        #    print p
        #    print "Converting ", p
        #    converter(p)
         print sys.argv[1]
         print "Converting ", sys.argv[1]
         converter(sys.argv[1])
