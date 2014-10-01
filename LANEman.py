#!/usr/bin/env python
# TODO: Write the actual script so it works with C++ plugins
# Plugins will go into ./build/bin when compiled as of the moment
# Add support for a new type of configuration file
# Consider automating compilation of plugins when necessary
# Consider adding python plugin support

"""The plugin manager for LUCID data analysis"""
import os, sys, ConfigParser, operator, time, datetime

pluginPath = "plugins"

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


if __name__ == "__main__":
    os.system("make all")
    plugins = getPlugins()
    for p in plugins:
        print datetime.datetime.fromtimestamp(time.time()).strftime(
                "%Y-%m-%d %H:%M:%S")
        print "Running: " + p.name
        if p.language.lower() == "cpp" or p.language.lower() == "c":
            os.system("build/bin/" + p.name)
        elif p.language.lower() == "py" or p.language.lower() == "python":
            os.system("python build/bin/" + p.name + ".py")
        else:
            print "Invalid language option set for '" + p.name + "' in config"
            sys.exit(1)

