#!/usr/bin/env python2
# Plugins will go into ./build/bin when compiled as of the moment
# Consider adding proper plugin dependency handling

"""The plugin manager for LUCID data analysis"""
import os, sys, ConfigParser, operator, time, datetime, errno

# Configurable constants
binariesPath = os.path.abspath('./bin')
resultsPath = os.path.abspath('./results')
dataPath = os.path.abspath('./data')

outputPath = '"' + resultsPath + '/"'

class Settings:
    """A settings object, containing LANE metadata"""
    def __init__(self, project, version):
        self.project = project
        self.version = version

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
    globalSettings = Settings("ERROR", 0.0)
    for name in pluginConfig.sections():
        data = getSectionData(pluginConfig, name)
        if name == "GlobalSettings":
            globalSettings = Settings(data["project"], float(data["version"]))
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
    
def listDirFullPath(d):
    finalList = []
    for f in os.listdir(d):
        if os.path.isdir(os.path.join(d, f)):
            finalList.append(os.path.join(d, f))
    return finalList

def runPlugins():
    """Runs the plugins"""
    mkdirP(resultsPath)
    globalSettings, plugins = getPlugins()
    for p in plugins:
        if (p.project.lower() == "all") or (p.project.lower() == globalSettings.project.lower()):    
            print datetime.datetime.fromtimestamp(time.time()).strftime(
                    "%Y-%m-%d %H:%M:%S")
            print "Running: " + p.name

            for d in listDirFullPath(dataPath) + [dataPath]:
                print d
                pluginParameters = "\"" + d + "/\"" + " " + outputPath
                if p.language.lower() == "cpp" or p.language.lower() == "c":
                    os.system(binariesPath + "/" + p.name + " " + pluginParameters)
                elif p.language.lower() == "py" or p.language.lower() == "python":
                    os.system("python2 \"" + binariesPath + "/" + p.name + ".py\"" + " " + pluginParameters)
                else:
                    print "Invalid language option set for '" + p.name + "' in config"
                    sys.exit(1)
        print ""

if __name__ == "__main__":
    runPlugins()
