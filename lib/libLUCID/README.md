# LibLUCID
A library containing general utilities necessary for the LUCID project using 
C++11, Doxygen and GoogleTest.


## Prerequisites and installation
To build this code you will need

* [CMake][] build system
* A C++11 compiler
* (Optionally) [Doxygen][]


## Building
First, you will need an installation of [CMake][].
Once this is done, you can `cd` to the root of
the project and setup the build directory:

```shell
mkdir build
cd build
cmake ..
```

or you can just:

```shell
make gen-cmake
```

You can now build and run the project.
The following commands may differ for different compilers/build-systems:

```shell
make all
```


## Documentation
Documentation can be built using:

```shell
make doc
```


## Tests
Tests can be built and run using:

```shell
make test
```


## Project organization
The project is organized in a couple of subdirectories.

* The [doc](doc) directory contains configuration files needed to generate
  the documentation.
* The [lib](lib) directory contains most of the external and internal
dependencies needed for building.
* The [include](include) directory contains the public header files for the 
library.
* The [test](test) directory contains the source files for the unit tests.


## License
Issued under the Simplified (2-clause) BSD license.
Please see [LICENSE.md](LICENSE.md).


##External Libraries:

[GoogleTest][] - Under New BSD license


<!-- Links -->
[Doxygen]: http://www.doxygen.org
[CMake]: http://www.cmake.org
[GoogleTest]: https://code.google.com/p/googletest/

