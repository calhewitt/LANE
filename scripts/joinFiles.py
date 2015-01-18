#!/usr/bin/env python2
# Joins up files in the order given
import sys, os

def joinFiles(files):
    if len(files) < 2:
        return

    # Read in and append all file data to a new file in order given
    out = open(files[0] + '_joined', 'wb')
    for f in files:
        input = open(f, 'rb')
        while True:
            data = input.read(1024**2)
            if not data:
                break
            out.write(data)
        input.close
    out.close()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'USAGE: %s inputfile1 inputfile2 [...]' % (str(sys.argv[0]))
        sys.exit(1)
    
    # Get tail of the list
    it = iter(sys.argv)
    it.next()
    files = list(it)
    print len(files)
    
    joinFiles(files)
    