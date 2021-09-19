# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:
MAKEFLAGS += --silent

SOURCE_DIR = .
BUILD_DIR = build
# GENERATOR = "MinGW Makefiles"

cmake:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DTAU_BUILDINTERNALTESTS=On
	cmake --build $(BUILD_DIR) --config Debug
	echo ------------------ Running Target ------------------
	cd build/bin/ ; ./TauInternalTests
.PHONY: cmake

script:
	python3 script.py
.PHONY: script

cmakeclean:
	rm -rf build/ && mkdir build
.PHONY: cmakeclean