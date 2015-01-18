#!/usr/bin/env python2
# Automatically helps the user setup a lane config.ini

import sys, os


configTemplate = """\
########################THIS-FILE-IS-GENERATED-BY-CONFIGLANE###################
# A configuration file for the plugin manager to use for managing the modules
# Format is a simple windows INI style
# The configuration is case-sensitive
# Language is the name of the programming language used in the module
# Stage number is the order in which the modules are run. This is a rough
# method of dependency handling, and will be supplanted by more complex graph
# operations in future.
# 'project' is the name of the group the module should be activated for,
# so for example RAY, LUCID etc.
# Set a module's 'project' setting to all to make it always active
# Required format:
# [moduleName]
# project: groupName
# author: nameOfAuthor
# license: nameOfLicense
# language: cpp/c/python/py
# stage: stageNumber

[GlobalSettings]
version: 0.1
project: lucid
resultsPath: ./results
dataPath: ./data
modulesPath: ./modules
masksPath: ./masks
calibrationsPath: ./calibrations
configurationsPath: ./configurations

[rawToIntermediate]
project: all
author: Hector Stalker
license: BSD 2-clause
language: cpp
stage: 0

[basicClusterAnalysis]
project: all
author: Hector Stalker & Sam Kittle
license: BSD 2-clause
language: cpp
stage: 1

[pairing]
project: lucid
author: Sam Kittle
license: BSD 2-clause
language: python
stage: 2

"""

if __name__ == '__main__':
    print "--***[=Configure lane=]***--"
    print "Welcome to the lane configuration program!"
    print "Todo: Write the rest of this script to guide the user through generating a config.ini"
    print "At the moment just generate a pre-made config"
    
    # Generate a 
    print "Generating config.ini..."
    f = open('config.ini', 'wb')
    f.write(configTemplate)
    f.close()
    print "Complete!"
    sys.exit(0)
