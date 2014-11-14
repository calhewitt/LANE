# LANE (Langton ANalysis Ecosystem)
A software ecosystem for analysis of the data from the Langton physics projects 
For example, the LUCID(Langton's Ultimate Cosmic-ray Detector) project onboard TechDemoSAT-1 in space.

This software package contains:

* A plugin management/automation python script.
* A selection of analysis plugins.
* A library of C++ code for use in analysis of Langton physics data.


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
make tools plugins
```

Then run:
```shell
python2 ./scripts/InstallLANE.py ./ output-path
```
where output-path is the path to install the LANE main binaries to.


## Running libLANE Tests
Use the following to build and then run libLANE tests:
```shell
make check
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
* The [lib/libLANE](lib/libLANE) and [lib/libLANEpy](lib/libLANEpy) directory
contains a library of code for use in the plugins.
* The [doc](doc) directory contains the configuration files for documentation 
generation.
* The [testdata](testdata) directory contains some uncompressed test data.
* The [scripts](scripts) directory contains some possibly useful maintenance scripts.



## License
Issued under the Simplified (2-clause) BSD license.
Please see [LICENSE.md](LICENSE.md).


## External Libraries:

[GoogleTest][] - Under New BSD license


## TODO

* Add support for XYV compression to libLANE.
* Add support for some more of the settings in the data files' header to 
libLANE.
* Add telemetry data/metadata support to libLANE.


<!-- Links -->
[Doxygen]: http://www.doxygen.org
[CMake]: http://www.cmake.org
[GoogleTest]: https://code.google.com/p/googletest/
[Python2]: https://www.python.org/
