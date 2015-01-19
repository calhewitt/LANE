# LANE (Langton ANalysis Ecosystem)
A software ecosystem for analysis of the data from the Langton physics projects 
For example, the LUCID (Langton's Ultimate Cosmic-ray Detector) project onboard 
TechDemoSAT-1 in space.

This software package contains:

* A lane management/automation python scripts.
* A selection of analysis modules.
* A library of C++ code for use in analysis of Langton physics data.

See the [LUCID wiki][] for further information.


## Prerequisites and installation
To build this code you will need

* [CMake][] build system
* A C++11 compiler
* [Python2][]


## Building
* Enter the lane directory in your shell:
```shell
cd lane-directory
```
* Run [setupBuild](setupBuild.py), passing it the path to which you would like 
lane to be installed:
```shell
./setupBuild.py install-path
```
The [setupBuild](setupBuild.py) script can also be forced to generate build 
files for a specific build system. See the [setupBuild](setupBuild.py) script's 
usage information for more information:
```shell
./setupBuild.py --help
```
* Enter the desired build directory inside the lane directory and build the 
project. For example, on a unix based system:
```shell
cd build/release
make install
```
This will build the project and install the results in the earlier specified 
directory.


## Running
Use [configLane](configLane.py) to generate a config.ini file before running 
lane:
```shell
./configLane.py
```

Any LUCID data you wish to be analysed should be placed inside the data 
directory, inside the lucid folder, within a folder named like so: year-month-day, 
inside another folder with an ID identifying the capture number of that data. For 
example: data/lucid/2015-01-15/01.

Use [runLane](runLane.py) to run the analysis modules on the data:
```shell
./runLane.py
```


## Useful scripts
Inside the [scripts](scripts) directory are several useful scripts for lane 
development and usage.

* [sortLucidCaptures.py](sortLucidCaptures.py) is a script that 
scans all files in a directory with the *.ldat extension and joins up files 
determined to be members of the same overall capture file.

* [rawToIntermediate](rawToIntermediate.py) is an (in development) script for 
reading in LUCID raw data files (currently doesn't work).


## Notes for when making additions
It's a good idea to make your desired module code changes in a lane installation,
and backport these changes to the source location before committing the code. 
This means it will be possible to test run the code. In the base source location 
prior to installation, lane is not designed to be run!


## Project organization
The main directory contains some information and the lane configuration script, 
[configLane](configLane.py)
The project is also organized into a couple of subdirectories.

* The [modules](modules) directory contains the source code of 
the analysis modules.
* The [lib](lib) directory contains the libraries used in the software.
* The [lib/lane](lib/lane) directory contains C++-based libraries of code for 
use in the analysis modules.
* The [scripts](scripts) directory contains some possibly useful maintenance 
scripts.
* The [cmake](cmake) directory contains some utility scripts for the cmake C++ 
build system.


## License
Issued under the Simplified (2-clause) BSD license.
Please see [LICENSE.md](LICENSE.md).


## External Libraries:
None as of yet


## TODO

-Urgent-

* Fix the parser for lucid files to allow for reading in unjoined files of 
arbitrary size - Modify the raw-to-intermediate conversion modules to allow for 
this.

* Figure out a sound way of creating a python library for use in modules (plane)  
that's sanely accessible from python modules, or decide if it's worth just 
outright writing every module in C++?


-Less Urgent-

* Add support for XYV compression to liblane.

* Add support for some more of the settings in the data files' header to 
liblane.

* Add telemetry, detector configs, pixel masks and calibration files support 
to lane.


<!-- Links -->
[CMake]: http://www.cmake.org
[Python2]: https://www.python.org/
[LUCID wiki]: http://starserver.thelangton.org.uk/wiki/index.php/LUCID
