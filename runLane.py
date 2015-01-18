#!/usr/bin/env python2

"""The module manager for LUCID data analysis"""
import os, sys, ConfigParser, operator, time, datetime, errno

modulesPath = ''
resultsPath = ''
dataPath = ''
masksPath = ''
calibrationsPath = ''
configurationsPath = ''

class Settings:
    """A settings object, containing LANE metadata"""
    def __init__(self, project, version, modulesPath, resultsPath, dataPath, masksPath, calibrationsPath, configurationsPath):
        self.project = project
        self.version = version
        self.modulesPath = os.path.abspath(modulesPath)
        self.resultsPath = os.path.abspath(resultsPath)
        self.dataPath = os.path.abspath(dataPath)
        self.masksPath = os.path.abspath(masksPath)
        self.calibrationsPath = os.path.abspath(calibrationsPath)
        self.configurationsPath = os.path.abspath(configurationsPath)

class Module:
    """A Module object, containing module metadata"""
    def __init__(self, project, name, author, license, language, stage):
        self.project = project
        self.name = name
        self.author = author
        self.license = license
        self.language = language
        self.stage = stage

def getSectionData(config, section):
    """Generates a map of key-values for a given section of an INI"""
    dict1 = {}
    options = config.options(section)
    for option in options:
        try:
            dict1[option] = config.get(section, option)
        except:
            print("exception on %s!" % option)
            dict1[option] = None
    return dict1

def sortModules(modules):
    """Sorts a given list of module objects by their stage"""
    return sorted(modules, key=operator.attrgetter("stage"))

def getModules():
    """Gets a list of Module objects containing all the module info
from the config file and the global settings for LANE"""
    moduleConfig = ConfigParser.ConfigParser()
    moduleConfig.read("config.ini")
    moduleList = []
    globalSettings = Settings("ERROR", 0.0, "", "", "", "", "", "")
    for name in moduleConfig.sections():
        data = getSectionData(moduleConfig, name)
        if name == "GlobalSettings":
            globalSettings = Settings(data["project"], float(data["version"]),
                data["modulespath"],
                data["resultspath"],
                data["datapath"],
                data["maskspath"],
                data["calibrationspath"],
                data["configurationspath"])
        else:
            module = Module(data["project"],
                name,
                data["author"],
                data["license"],
                data["language"], 
                data["stage"])
            moduleList.append(module)
    return (globalSettings, sortModules(moduleList))

def mkdirP(path):
    """Makes a directory - basically provides mkdir -p shell-like functionality"""
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise

def getDataFilePaths(d):
    finalList = []
    for root, dirs, files in os.walk(d):
        for dir in dirs:
            finalList.append(os.path.join(root, dir))
    return finalList

def runModules():
    """Runs the Modules"""
    globalSettings, modules = getModules()
    outputPath = globalSettings.resultsPath
    mkdirP(globalSettings.resultsPath)
    mkdirP(globalSettings.dataPath)
    mkdirP(globalSettings.modulesPath)
    mkdirP(globalSettings.masksPath)
    mkdirP(globalSettings.calibrationsPath)
    mkdirP(globalSettings.configurationsPath)

    for m in modules:
        if (m.project.lower() == "all") or (m.project.lower() == globalSettings.project.lower()):
            print datetime.datetime.fromtimestamp(time.time()).strftime(
                    "%Y-%m-%d %H:%M:%S")
            print "Running: " + m.name

            for d in getDataFilePaths(globalSettings.dataPath):
                print d
                currentOutputPath = os.path.join(outputPath, os.path.basename(os.path.normpath(d)))
                mkdirP(currentOutputPath)
                moduleParameters = '"' + d + '/" ' + \
                    '"' + currentOutputPath + '/" ' + \
                    '"' + globalSettings.masksPath + '/" ' + \
                    '"' + globalSettings.calibrationsPath + '/" ' + \
                    '"' + globalSettings.configurationsPath + '/"'
                if m.language.lower() == "cpp" or m.language.lower() == "c":
                    os.system(os.path.join(globalSettings.modulesPath, m.name, m.name + " " + moduleParameters))
                elif m.language.lower() == "py" or m.language.lower() == "python":
                    os.system(os.path.join("python2 \"" + globalSettings.modulesPath, m.name, m.name + ".py\"" + " " + moduleParameters))
                else:
                    print "Invalid language option set for '" + m.name + "' in config"
                    sys.exit(1)
        print ""

if __name__ == "__main__":
    runModules()
