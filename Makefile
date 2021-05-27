.PHONY: all echo

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:

all:
	python script.py 
.PHONY: all

echo:
	echo Working!
.PHONY: echo 

# ---------- CMAKE STUFF --------
TARGET = Hazel
SOURCE_DIR = .
BUILD_DIR = build
GENERATOR = "MinGW Makefiles" 

# PROCESS:
## 1. CMake Exec (generate the build files)
## 2. Run CMake Makefile
## 3. Run CXX compiled executable
 
cmake:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -G $(GENERATOR)
	echo --------------------------------------------
	echo --------------------------------------------
	cmake --build build
	echo --------------------------------------------
	echo --------------------------------------------
	cd build/test/InternalTests && MuonInternalTests
.PHONY: cmake 

cmakemsvc:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	echo --------------------------------------------
	echo --------------------------------------------
	cmake --build build
	echo --------------------------------------------
	echo --------------------------------------------
	cd build/test/InternalTests/Debug && MuonInternalTests
.PHONY: cmakemsvc

cmakeclean:
	rmdir /Q /S $(BUILD_DIR) && mkdir $(BUILD_DIR)
.PHONY: cmakeclean


test:
	gcc test/test.c -o test -I . -I Muon -std=c11
	echo ----------------- Compiled! -----------------
	test
.PHONY: test

acutest:
	gcc acutest.c -o acutest -I . -I Muon -std=c11
	echo ----------------- Compiled! -----------------
	acutest 
.PHONY: acutest