#!/usr/bin/env python2
# Copies required LANE files to the given directory from the given directory
import sys, os, shutil

def mkdir(path):
    if not os.path.exists(path):
        os.mkdir(path)

def copyIfExists(inputPath, outputPath, recursive = False):
    if not os.path.exists(inputPath):
        raise IOError("Unable to copy '" + inputPath + "'")
    if recursive:
        try:
            shutil.copytree(inputPath, outputPath)
        except Exception as e:
            shutil.rmtree(outputPath)
            shutil.copytree(inputPath, outputPath)
    else:
        shutil.copyfile(inputPath, outputPath)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print 'USAGE: %s input-path output-path' % (str(sys.argv[0]))
        sys.exit(1)
    
    inputPath = os.path.abspath(sys.argv[1])
    outputPath = os.path.abspath(sys.argv[2])
    try:
        mkdir(outputPath)
        mkdir(outputPath + '/results')
        mkdir(outputPath + '/data')
        copyIfExists(inputPath + '/build/bin', outputPath + '/bin', recursive = True)
        copyIfExists(inputPath + '/LANEman.py', outputPath + '/LANEman.py')
        copyIfExists(inputPath + '/plugins/plugins.ini', outputPath + '/plugins.ini')
    except Exception as e:
        print "Error: ", e
        sys.exit(1)
