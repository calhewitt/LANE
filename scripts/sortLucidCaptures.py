#!/usr/bin/env python2
"""Sorts the lucid files for a certain date into separate captures"""
import os, sys, glob, shutil, errno

def mkdirP(path):
    """Provides mkdir -p functionality

    :path: The path to create

    """
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

def copyFiles(paths, destination):
    """Copies the given files to the given destination directory

    :paths: The paths of the files to copy
    :destination: The destination directory for the files to be copied to

    """
    mkdirP(destination)
    for f in paths:
        shutil.copy2(f, os.path.join(destination, os.path.basename(f)))

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

def hasLucidHeader(filePath):
    """Checks whether a given file has a Lucid data file header

    :filePath: The path to the file to check
    :returns: True if the file contains a Lucid header, false otherwise

    """
    isLucidFile = False
    f = open(filePath, 'rb')
    magic = f.read(2)
    if ord(magic[0]) == 0xDC and ord(magic[1]) == 0xCC:
        isLucidFile = True
    f.close()
    return isLucidFile

def getTimeStamp(path):
    """Returns the time-stamp portion of the lucid filename strings, which are:
        T1_LU_SOMENUMBER_YYYYMMDD_HHMMSS

    :path: The file path string
    :returns: A string containing the time-stamp

    """
    base = os.path.basename(path)
    return os.path.splitext(base)[0].split('_')[4]

def sortByTimeStamp(paths):
    """Sorts the given list of file paths by their time-stamp

    :paths: The file paths to sort by time-stamp
    :returns: A sorted list of file paths

    """
    sortedPaths = []
    timeStamps = []

    # Extract the HHMMSS timestamps from the file paths
    for p in paths:
        timeStamps.append(int(getTimeStamp(p)))

    # Sort the timestamps in ascending order
    timeStamps.sort()

    # Sort the paths by comparing to the sorted timestamps
    for t in timeStamps:
        for p in paths:
            if int(getTimeStamp(p)) == t:
                sortedPaths.append(p)

    return sortedPaths

if __name__ == '__main__':
    if len(sys.argv) <= 1 or len(sys.argv) > 3:
        print 'USAGE: ' + sys.argv[0] + ' file-directory [output-directory]'
        sys.exit(1)

    # Figure out the destination path
    if len(sys.argv) == 3:
        destinationPath = os.path.abspath(sys.argv[2])
    else:
        destinationPath = os.path.abspath(sys.argv[1])

    # Grab files in the directory and sort them
    files = sortByTimeStamp(listFiles(sys.argv[1], 'ldat'))

    # Just check for the case in which no files are found
    if files == None:
        print 'Path ' + sys.argv[1] + ' is empty!'
        sys.exit(0)

    # Separate and copy the files into individual capture directories
    captureNo = 1
    currentCapture = []
    for f in files:
        if hasLucidHeader(f):
            print 'Capture ' + str(captureNo) + ':'
            print f

            if len(currentCapture) != 0:
                # Make a directory and copy the capture files into it
                copyFiles(currentCapture, os.path.join(destinationPath, str(captureNo)))
                currentCapture = []
                captureNo += 1
        else:
            print f

        currentCapture.append(f)

    # Copy out any remaining capture's files
    if len(currentCapture) != 0:
        copyFiles(currentCapture, os.path.join(destinationPath, str(captureNo)))

