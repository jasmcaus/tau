cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Arrow 
        VERSION 1.0.0
        DESCRIPTION "A Micro Unit Testing Framework for C/C++"
        LANGUAGES C)

# ------ Disable CMAKE_INSTALL_MESSAGE  ------
set(CMAKE_INSTALL_MESSAGE NEVER)

# ------ Check and set CMAKE_CXX_STANDARD ------
# string(FIND "${CMAKE_C_FLAGS}" "-std=c" ENV_CXX_STANDARD)
# if(ENV_CXX_STANDARD GREATER -1)
#     message (
#         WARNING "C Standard Version definition detected in an environment variable."
#         "Hazel requires -std=c11. Please remove -std=c settings in your environment."
#     )
# endif()

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## 
## Options
##

# Determine if Arrow is built as a subproject (using add_subdirectory) or if it is the main project being built
set(IS_MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(IS_MAIN_PROJECT ON)
endif()


if (POLICY CMP0077)
    # Allow CMake 3.13+ to override options when using FetchContent/add_subdirectory.
    cmake_policy(SET CMP0077 NEW)
endif()


option(ARROW_BUILDTESTS "Build unit tests." ${IS_MAIN_PROJECT})
option(ARROW_USE_CI "Enable CI Build Targets" OFF)

if(ARROW_USE_CI)
    include(cmake/CI.cmake)
endif()


## 
## Configurations
##

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
    string(APPEND CMAKE_C_FLAGS " /std:c11")
    string(APPEND CMAKE_C_FLAGS " /Wall")
    # string(APPEND CMAKE_C_FLAGS " /WX")  # Treats Linker Warnings as Errors
    string(APPEND CMAKE_C_FLAGS " /wd4514")
    string(APPEND CMAKE_C_FLAGS " /wd5045")
endif()


# This library will be platform-independent, but still be installable following best practices. 
# We include GNUInstallDirs (included in CMake) which provides us with a set of variables containing installation 
# directories for various artifacts.
include(GNUInstallDirs)

# ------ Options ------
set(ARROW_TARGET_NAME           ${PROJECT_NAME}Targets)
set(ARROW_TARGETS_CMAKE         ${CMAKE_CURRENT_BINARY_DIR}/${ARROW_TARGET_NAME}.cmake)

set(ARROW_HEADERS_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/Arrow)

# CMake Configurations (from the ``cmake`` folder)
set(CMAKE_CONFIGVERSION_IN      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ConfigVersion.cmake.in)
set(CMAKE_CONFIGVERSION         ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
set(CMAKE_CONFIG_IN             ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in)
set(CMAKE_CONFIG                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
set(CMAKE_CONFIGS_INSTALL_DIR   "lib/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(CMAKE_PKGCONFIG_IN          ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in)
set(CMAKE_PKGCONFIG             ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Pkg-Config.pc)


# ------ Main Build ------
# Main Build File for the main Arrow Library
# Note that this library must have _minimal_ dependencies 

# Where Google Test's .h files can be found.
set(ARROW_BUILD_INCLUDE_DIRS
    "${${PROJECT_NAME}_SOURCE_DIR}/Arrow"
    "${${PROJECT_NAME}_SOURCE_DIR}")
include_directories(${ARROW_BUILD_INCLUDE_DIRS})

# Create the Arrow Interface Library
# Interfaces are best for header-only libraries
add_library(${PROJECT_NAME} INTERFACE)
# Add an alias so that Arrow can be used with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    target_compile_features(${PROJECT_NAME} INTERFACE c_range_for)
else()
    target_compile_features(${PROJECT_NAME} INTERFACE c_std_11)
endif()

target_include_directories(
    ${PROJECT_NAME} 
    INTERFACE 
    # If Arrow is used directly by another CMake target (such as when building tests or when it is included 
    # as a sub-directory)
    # $<BUILD_INTERFACE:"${ARROW_HEADERS_INCLUDE_DIR}">
    $<BUILD_INTERFACE:${ARROW_BUILD_INCLUDE_DIRS}>
    # The path if Arrow is installed
    # $<INSTALL_INTERFACE:"include">
    $<INSTALL_INTERFACE:Arrow}>
)


# ------ Installations ------
## Installation header files, generate and install CMake config files for find_package()
include(CMakePackageConfigHelpers)

# We write a custom package version config file instead of `write_basic_package_version_file` to ensure that it's 
# architecture-independent

# ConfigVersion.cmake.in --> ArrowConfigVersion.cmake
configure_file(
    ${CMAKE_CONFIGVERSION_IN}
    ${CMAKE_CONFIGVERSION}
    @ONLY
)
# config.cmake.in --> ArrowConfig.cmake
configure_file(
    ${CMAKE_CONFIG_IN}
    ${CMAKE_CONFIG}
    @ONLY
)
# Install a pkg-config file, so other tools can find this.
configure_file(
    ${CMAKE_PKGCONFIG_IN}
    ${CMAKE_PKGCONFIG}
)
# Install the pkg-config file to lib/pkgconfig
install(
    FILES ${CMAKE_PKGCONFIG}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

# Move ArrowConfig.cmake and ArrowConfigVersion.cmake  ---> lib/cmake/Arrow/...
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" 
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

# ./include --> include
install(
    DIRECTORY ${ARROW_HEADERS_INCLUDE_DIR}
    DESTINATION Arrow
)


# Export ArrowTargets from ArrowTargets.cmake
export(
    TARGETS ${PROJECT_NAME}
    FILE ${ARROW_TARGETS_CMAKE}
)

install(
    EXPORT ${ARROW_TARGET_NAME}
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# install(
#     TARGETS ${PROJECT_NAME}
#     EXPORT "${ARROW_TARGET_NAME}Targets"
#     INCLUDES DESTINATION ${ARROW_HEADERS_INCLUDE_DIR}
# )
install(TARGETS ${PROJECT_NAME}
        EXPORT ${ARROW_TARGET_NAME}
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        ## This results in an ``INTERFACE_INCLUDE_DIRECTORIES`` property contains path ... which is prefixed in the source 
        ## directory. 
        ## There may be something I'm missing out - if you know how to fix this, please send in a PR :)
        # INCLUDES DESTINATION ${ARROW_HEADERS_INCLUDE_DIR)
)

# Header files are copied to the install directory
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
    DESTINATION include
)

# Building Tests
if(ARROW_BUILDTESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()