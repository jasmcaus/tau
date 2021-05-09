cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Arrow 
        LANGUAGES C)

# ------ Disable CMAKE_INSTALL_MESSAGE  ------
set(CMAKE_INSTALL_MESSAGE NEVER)

# ------ Check and set CMAKE_CXX_STANDARD ------
string(FIND "${CMAKE_C_FLAGS}" "-std=c" ENV_CXX_STANDARD)
if(ENV_CXX_STANDARD GREATER -1)
    message (
        WARNING "C Standard Version definition detected in an environment variable."
        "Hazel requires -std=c11. Please remove -std=c settings in your environment."
    )
endif()

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ------Compiler and linker options ------
# In case of Makefiles, if the user does not setup CMAKE_BUILD_TYPE, assume it's Release: 
if(CMAKE_GENERATOR MATCHES "Makefiles|Ninja" AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build from: Debug Release." FORCE)
endif()

# ------ A List of Compiler Flags ------
# A (more or less comprehensive) list is here: https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
if(NOT MSVC)
    if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        string(APPEND CMAKE_C_FLAGS " -std=gnu11")
    else()
        string(APPEND CMAKE_C_FLAGS " -std=c11")
    endif()
    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND CMAKE_C_FLAGS " -O2 -fPIC")
    endif()
    string(APPEND CMAKE_C_FLAGS " -Wall")
    string(APPEND CMAKE_C_FLAGS " -Wextra")
    string(APPEND CMAKE_C_FLAGS " -Werror=return-type")
    string(APPEND CMAKE_C_FLAGS " -Wno-unknown-pragmas")
    string(APPEND CMAKE_C_FLAGS " -Wno-sign-compare")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-parameter")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-variable")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-function")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-result")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-local-typedefs")
    string(APPEND CMAKE_C_FLAGS " -Wno-strict-overflow")
    string(APPEND CMAKE_C_FLAGS " -Wno-strict-aliasing")
    string(APPEND CMAKE_C_FLAGS " -Wno-error=deprecated-declarations")

    if(WERROR)
        check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)
        if(NOT COMPILER_SUPPORT_WERROR)
            set(WERROR FALSE)
        else()
            string(APPEND CMAKE_C_FLAGS " -Werror")   
        endif()
    endif(WERROR)

    if(NOT APPLE)
        string(APPEND CMAKE_C_FLAGS " -Wno-unused-but-set-variable")
        string(APPEND CMAKE_C_FLAGS " -Wno-maybe-uninitialized")
    endif()

# For MSVC 
else()
    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND CMAKE_C_FLAGS " /O2")
    endif()
    string(APPEND CMAKE_C_FLAGS " /Wall")
    # string(APPEND CMAKE_C_FLAGS " /WX")  # Treats Linker Warnings as Errors
    string(APPEND CMAKE_C_FLAGS " /wd4514")
    string(APPEND CMAKE_C_FLAGS " /wd5045")
endif()


# ------ Main Build ------
# Main Build File for the main Arrow Library
# 
# Note that this library must have _minimal_ dependencies 
set(ARROW_CMAKE_VERSION_CONFIG_FILE  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(ARROW_CMAKE_PROJECT_CONFIG_FILE  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(ARROW_CMAKE_PROJECT_TARGETS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

file(GLOB_RECURSE ARROW_HEADER_SOURCES Arrow/*.h)


# Create the Arrow Interface Library
# Interfaces are best for header-only libraries
add_library(Arrow INTERFACE ${ARROW_HEADER_SOURCES})
add_library(Arrow::Arrow ALIAS Arrow)

if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    target_compile_features(Arrow INTERFACE c_range_for)
else()
    target_compile_features(Arrow INTERFACE c_std_11)
endif()

target_include_directories(
    Arrow INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

# Where Google Test's .h files can be found.
set(ARROW_BUILD_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${ARROW_BUILD_INCLUDE_DIRS})

##
## INSTALL
# Create the CMake package file descriptors.
# use a custom package version config file instead of
# write_basic_package_version_file to ensure that it's architecture-independent
# https://github.com/nlohmann/json/issues/1697

install(
        TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(ARROW_BUILDTESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()