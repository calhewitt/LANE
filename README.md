# LANE (LUCID ANalysis Ecosystem)
A software ecosystem for analysis of the data from the LUCID 
(Langton's Ultimate Cosmic-ray Detector) project onboard TechDemoSAT-1 in space.

This software package contains:

* A plugin management/automation python script.
* A selection of analysis plugins.
* A library of C++ code for use in analysis of LUCID data.


## Prerequisites and installation
To build this code you will need

* [CMake][] build system
* A C++11 compiler
* [Python2][]
* (Optionally) [Doxygen][]


## Building
First, you will need an installation of [CMake][].
Once this is done, you can `cd` to the root of
the project.
You can now build and run the project.
The following commands may differ for different compilers/build-systems:

```shell
python LANEman.py
```


## Running libLUCID Tests
Use the following to build and then run libLUCID tests:
```shell
make check
```


## Building LANE tools
Use the following to build the LANE tool suite:
```shell
make tools
```


## Project organization
The main directory contains some information and the plugin manager script, 
[LANEman](LANEman.py)
The project is also organized into a couple of subdirectories.

* The [plugins](plugins) directory contains the source code and binaries of 
the analysis plugins, as well as the configuration files for the plugin 
manager.
* The [tools](tools) directory contains the LANE-based tools/software.
* The [lib](lib) directory contains the libraries used in the software.
* The [lib/external](lib/external) directory contains the third-party 
libraries used in this software.
* The [lib/libLUCID](lib/libLUCID) directory contains a library of code for 
use in the plugins.
* The [doc](doc) directory contains the configuration files for documentation 
generation.
* The [testdata](testdata) directory contains some uncompressed test data.


## How to add plugins (TODO)
Copy [plugins/testPlugin](plugins/testPlugin) and change the name to the plugin 
name in the CMakeLists.txt, update the README.md, add the code files to 
the CMakeLists.txt, update LICENSE.md, and finally write the code in src.

Python plugins can be added by copying the 
[plugins/testPluginPython](plugins/testPluginPython) directory and modifying 
the CMakeLists.txt to reference your plugin scripts.

Remember to add the details of your plugin to
[plugins/plugins.conf](plugins/plugins.ini).


## License
Issued under the Simplified (2-clause) BSD license.
Please see [LICENSE.md](LICENSE.md).


## External Libraries:

[GoogleTest][] - Under New BSD license


## TODO

* Add support for XYV compression to libLUCID.
* Add support for some more of the settings in the data files' header to 
libLUCID.
* Add telemetry data/metadata support to libLUCID.


<!-- Links -->
[Doxygen]: http://www.doxygen.org
[CMake]: http://www.cmake.org
[GoogleTest]: https://code.google.com/p/googletest/
[Python2]: https://www.python.org/
