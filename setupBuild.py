#!/usr/bin/env python2
# Sets up the build files and directories for the building of lane
import sys, os, subprocess, shutil

def printUsage():
    print "USAGE: " + sys.argv[0] + " installDirectory [buildSystem]"
    print "installDirectory -> the directory in which installation should place lane"
    print "Options for optional parameter buildSystem:"
    print "- unix or linux -> Unix Makefiles"
    print "- vc++ or vc or windows or win or msvc -> Microsoft Visual C++"
    print "- msys or mingw -> MSYS/MINGW"

def mkdir(path):
    if not os.path.exists(path):
        os.mkdir(path)

if __name__ == '__main__':
    print "--***[=Setup lane build=]***--"
    print "Welcome to the lane build setup program!"
    
    generator = 'error'
    # Check the parameters
    if len(sys.argv) == 3:
        print "Automatically determining build system..."
        # Use a user given string to specify the build system to use
        if sys.argv[2].lower() in ['unix', 'linux']:
            generator = "Unix Makefiles"
        elif sys.argv[2].lower() in ['vc++', 'vc', 'windows', 'win', 'msvc']:
            generator = "Visual Studio 14 2015"
        elif sys.argv[2].lower() in ['msys', 'mingw']:
            generator = "MSYS Makefiles"
        else:
            print "Incorrect parameter '" +  + "'!"
            printUsage()
            sys.exit(1)
    elif len(sys.argv) == 2:
        # Default to an appropriate generator string
        if os.name == 'posix' and sys.platform == 'msys':
            generator = "MSYS Makefiles"
        elif os.name == 'posix' and not (sys.platform == 'msys'):
            generator = "Unix Makefiles"
        elif os.name == 'nt':
            generator = "Visual Studio 14 2015"
    else:
        # Incorrect number of parameters passed
        printUsage()
        sys.exit(1)
        
    # Handle help case by printing usage information
    if len(sys.argv) == 2 and sys.argv[1].lower() in ['help', '--help', '-h']:
        printUsage()
        sys.exit(0)
        
    print "Using '" + generator + "'"
    
    installPath = os.path.abspath(sys.argv[1])

    # Create build directories and run cmake
    print "Clearing build directory..."
    shutil.rmtree('./build', True)
    print "Creating build directory..."
    mkdir('./build')
    os.chdir('./build')

    print "Creating build/release directory..."
    mkdir('./release')
    os.chdir('./release')
    print "Configuring build files..."
    subprocess.call(['cmake', 
        '-Wno-dev',
        '-DCMAKE_BUILD_TYPE=Release', 
        '-DCMAKE_INSTALL_PREFIX:PATH=' + installPath, 
        generator, 
        '../../'])
    os.chdir('..')

    print "Creating build/debug directory..."
    mkdir('./debug')
    os.chdir('./debug')
    print "Configuring build files..."
    subprocess.call(['cmake', 
        '-Wno-dev',
        '-DCMAKE_BUILD_TYPE=Debug', 
        '-DCMAKE_INSTALL_PREFIX:PATH=' + installPath,
        generator,
        '../../'])
    
    os.chdir('../../')
