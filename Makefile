# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:

all:
	python3 script.py
.PHONY: all

SOURCE_DIR = .
BUILD_DIR = build
# GENERATOR = "MinGW Makefiles"

cmake:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DTAU_BUILDINTERNALTESTS=On
	cmake --build $(BUILD_DIR) --config Debug
	echo ------------------ Running Target ------------------
	cd build/bin/ ; ./TauInternalTests
.PHONY: cmake

cmakeclean:
	rm -rf build/ && mkdir build
.PHONY: cmakeclean