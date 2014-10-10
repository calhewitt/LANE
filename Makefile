# Simple Makefile to ease day-to-day development

#BUILD_TYPE=Debug
BUILD_TYPE=Release

.PHONY: clean all doc check tests gen-cmake clean-cmake

${VERBOSE}.SILENT:

%:
	make -C build $@

clean: gen-cmake
	make -C build $@

all: gen-cmake
	make -C build $@

doc: gen-cmake
	make -C build $@

tests: gen-cmake
	make -C build $@

check: gen-cmake
	make -C build $@

clean-cmake:
	rm -rf build

gen-cmake:
	mkdir -p build
	cd build && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -Wno-dev
	cd ..
