# Simple Makefile to ease day-to-day development

BUILD_TYPE=Debug
#BUILD_TYPE=Release

.PHONY: clean all test gen-cmake clean-cmake

${VERBOSE}.SILENT:

%:
	make -C build $@

clean:
	make -C build $@

all: gen-cmake
	make -C build $@

test: all
	cd build && ctest -V .

clean-cmake:
	rm -rf build

gen-cmake:
	mkdir -p build
	cd build && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -Wno-dev
	cd ..
