#!/usr/bin/env python2
# Plugins will go into ./build/bin when compiled as of the moment
# Consider adding proper plugin dependency handling

"""The plugin manager for LUCID data analysis"""
import os, sys, ConfigParser, operator, time, datetime, errno

binariesPath = ''
resultsPath = ''
dataPath = ''

class Settings:
    """A settings object, containing LANE metadata"""
    def __init__(self, project, version, binariesPath, resultsPath, dataPath):
        self.project = project
        self.version = version
        self.binariesPath = os.path.abspath(binariesPath)
        self.resultsPath = os.path.abspath(resultsPath)
        self.dataPath = os.path.abspath(dataPath)

class Plugin:
    """A plugin object, containing plugin metadata"""
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

def sortPlugins(plugins):
    """Sorts a given list of Plugin objects by their stage"""
    return sorted(plugins, key=operator.attrgetter("stage"))

def getPlugins():
    """Gets a list of Plugin objects containing all the plugin info
from the config file and the global settings for LANE"""
    pluginConfig = ConfigParser.ConfigParser()
    pluginConfig.read("plugins.ini")
    pluginList = []
    globalSettings = Settings("ERROR", 0.0, "", "", "")
    for name in pluginConfig.sections():
        data = getSectionData(pluginConfig, name)
        if name == "GlobalSettings":
            globalSettings = Settings(data["project"], float(data["version"]), data["binariespath"],
                data["resultspath"], data["datapath"])
        else:
            plugin = Plugin(data["project"], name, data["author"], data["license"],
                data["language"], data["stage"])
            pluginList.append(plugin)
    return (globalSettings, sortPlugins(pluginList))

def mkdirP(path):
    """Makes a directory - basically provides mkdir -p shell-like functionality"""
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise
    
def getDataFilePaths(d):
    finalList = [d]
    for root, dirs, files in os.walk(d):
        for dir in dirs:
            finalList.append(os.path.join(root, dir))
    return finalList


def runPlugins():
    """Runs the plugins"""
    globalSettings, plugins = getPlugins()
    outputPath = '"' + globalSettings.resultsPath + '/"'
    mkdirP(globalSettings.resultsPath)
    mkdirP(globalSettings.dataPath)
    mkdirP(globalSettings.binariesPath)
    for p in plugins:
        if (p.project.lower() == "all") or (p.project.lower() == globalSettings.project.lower()):    
            print datetime.datetime.fromtimestamp(time.time()).strftime(
                    "%Y-%m-%d %H:%M:%S")
            print "Running: " + p.name

            for d in getDataFilePaths(globalSettings.dataPath):
                print d
                pluginParameters = "\"" + d + "/\"" + " " + outputPath
                if p.language.lower() == "cpp" or p.language.lower() == "c":
                    os.system(globalSettings.binariesPath + "/" + p.name + " " + pluginParameters)
                elif p.language.lower() == "py" or p.language.lower() == "python":
                    os.system("python2 \"" + globalSettings.binariesPath + "/" + p.name + ".py\"" + " " + pluginParameters)
                else:
                    print "Invalid language option set for '" + p.name + "' in config"
                    sys.exit(1)
        print ""

if __name__ == "__main__":
    runPlugins()
