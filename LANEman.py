#!/usr/bin/env python2
# Plugins will go into ./build/bin when compiled as of the moment
# Consider adding proper plugin dependency handling

"""The plugin manager for LUCID data analysis"""
import os, sys, ConfigParser, operator, time, datetime, errno

# Configurable constants
binariesPath = './build/bin/'
pluginPath = os.path.abspath('plugins')
resultsPath = os.path.abspath('./results')
dataPath = os.path.abspath('./testdata')

# Internal constants
#inputPath = '"' + dataPath + '/"'
outputPath = '"' + resultsPath + '/"'
#pluginParameters = inputPath + " " + outputPath

def mkdir_p(path):
    """Makes a directory - basically provides mkdir -p shell-like functionality"""
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise

class Plugin:
    """A plugin object, containing plugin metadata"""
    def __init__(self, name, author, license, language, stage):
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
from the config file"""
    pluginConfig = ConfigParser.ConfigParser()
    pluginConfig.read(pluginPath + "/plugins.ini")
    pluginList = []
    for name in pluginConfig.sections():
        data = getSectionData(pluginConfig, name)
        plugin = Plugin(name, data["author"], data["license"],
                data["language"], data["stage"])
        pluginList.append(plugin)
    return sortPlugins(pluginList)

def runPlugins():
    """Builds and runs the plugins"""
    os.system("make plugins")
    mkdir_p(resultsPath)
    plugins = getPlugins()
    for p in plugins:
        print datetime.datetime.fromtimestamp(time.time()).strftime(
                "%Y-%m-%d %H:%M:%S")
        print "Running: " + p.name

        for d in os.listdir(dataPath) + [dataPath]:
            pluginParameters = "\"" + d + "/\"" + " " + outputPath
            if p.language.lower() == "cpp" or p.language.lower() == "c":
                os.system(binariesPath + p.name + " " + pluginParameters)
            elif p.language.lower() == "py" or p.language.lower() == "python":
                os.system("python2 \"" + binariesPath + "/" + p.name + "/" + p.name + ".py\"" + " " + pluginParameters)
            else:
                print "Invalid language option set for '" + p.name + "' in config"
                sys.exit(1)


if __name__ == "__main__":
    runPlugins()
