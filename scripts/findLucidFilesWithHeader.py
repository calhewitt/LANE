#!/usr/bin/env python2
# Finds all ldat files in a directory that have a proper file header
import os, sys

def findLucidFilesWithHeader(inputDirectory):
    """ Searches the .ldat files in a directory and returns a list of the files 
containing proper LUCID raw data file headers at the start"""
    filesWithHeader = []
    
    for root, dirs, files in os.walk(inputDirectory):
        for file in files:
            if file.endswith(".ldat"):
                f = open(os.path.join(root, file), 'rb')
                # Check for magic bytes
                magic = f.read(2)
                if ord(magic[0]) == 0xDC and ord(magic[1]) == 0xCC:
                    filesWithHeader.append(file)
                f.close()
    
    return filesWithHeader

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "USAGE: " + sys.argv[0] + " input-directory"
        sys.exit(1)
    
    count = 0
    for f in findLucidFilesWithHeader(os.path.abspath(sys.argv[1])):
        count += 1
        print str(count) + " - " + f
        
    print "Found a total of " + str(count) + " files with LUCID headers"
    sys.exit(0)
