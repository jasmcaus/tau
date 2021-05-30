CMake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(MUON_VERSION 1.0.0)

project(Muon 
        VERSION ${MUON_VERSION}
        DESCRIPTION "A Micro Unit Testing Framework for C/C++"
        LANGUAGES C CXX
)

# CMake's FATAL_ERROR is ignored on CMake versions >2.6 
# Hence we explicitely check for correct enforcement of CMake configurations.
if (${CMAKE_VERSION} VERSION_LESS "3.15")
    message(
        ERROR "You are currently on CMake Version ${CMAKE_VERSION}, but Muon supports CMake 3.15 above."
        "Please update your CMake installation."
    )
endif()

        
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
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## 
## Options
##

# Determine if Muon is built as a subproject (using add_subdirectory) or if it is the main project being built
set(IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_MAIN_PROJECT ON)
endif()


if (POLICY CMP0077)
    # Allow CMake 3.13+ to override options when using FetchContent/add_subdirectory.
    CMake_policy(SET CMP0077 NEW)
endif()


option(MUON_BUILDINTERNALTESTS "Build unit tests." ${IS_MAIN_PROJECT})
option(MUON_BUILDTHIRDPARTYTESTS "Build third party tests." OFF})
option(MUON_USE_CI "Enable CI Build Targets" OFF)
option(MUON_HIDE_INTERNAL_SYMBOLS "Hide internal symbols" ON)

if(MUON_USE_CI)
    include(CMake/CI.CMake)
endif()


if (MUON_HIDE_INTERNAL_SYMBOLS)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()


## 
## Configurations
##

# ------Compiler and linker options ------
# In case of Makefiles, if the user does not setup CMAKE_BUILD_TYPE, assume it's Release: 
if(CMAKE_GENERATOR MATCHES "Makefiles|Ninja" AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "[INFO] Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build from: [Debug, Release, RelWithDebInfo, MinSizeRel]." FORCE)
endif()

# ------ A List of Compiler Flags ------
# A (more or less comprehensive) list is here: https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html

if(NOT MSVC)
    if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        string(APPEND CMAKE_C_FLAGS " -std=gnu11")
    else()
        string(APPEND CMAKE_C_FLAGS " -std=c11")
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        string(APPEND CMAKE_CXX_FLAGS " -std=gnu++11")
    else()
        string(APPEND CMAKE_CXX_FLAGS " -std=c++11")
    endif()

    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND COMPILE_FLAGS " -O2")
    endif()
    
    string(APPEND COMPILE_FLAGS " -Wall")
    string(APPEND COMPILE_FLAGS " -Wextra")
    string(APPEND COMPILE_FLAGS " -Wno-unknown-pragmas")
    string(APPEND COMPILE_FLAGS " -Wno-sign-compare")
    string(APPEND COMPILE_FLAGS " -Wno-unused-parameter")
    # string(APPEND COMPILE_FLAGS " -Wno-unused-variable")
    # string(APPEND COMPILE_FLAGS " -Wno-unused-function")
    string(APPEND COMPILE_FLAGS " -Wno-unused-result")
    string(APPEND COMPILE_FLAGS " -Wno-unused-local-typedefs")
    string(APPEND COMPILE_FLAGS " -Wno-strict-overflow")
    string(APPEND COMPILE_FLAGS " -Wno-strict-aliasing")
    string(APPEND COMPILE_FLAGS " -Werror=return-type")
    string(APPEND COMPILE_FLAGS " -Wno-error=deprecated-declarations")

    if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
        # A hack for Clang on Ubuntu
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            string(APPEND CMAKE_CXX_FLAGS " -fPIE")
        endif() 

        string(APPEND COMPILE_FLAGS " -fPIE")
        string(APPEND COMPILE_FLAGS " -Wno-ignored-qualifiers")
        string(APPEND COMPILE_FLAGS " -Wno-uninitialized")
        string(APPEND COMPILE_FLAGS " -Wno-unused-const-variable")
        string(APPEND COMPILE_FLAGS " -Wno-newline-eof")
    else()
        string(APPEND COMPILE_FLAGS " -fPIC")
        string(APPEND COMPILE_FLAGS " -Wno-discarded-qualifiers")
        string(APPEND COMPILE_FLAGS " -Wno-multistatement-macros")

        if(NOT APPLE)
            string(APPEND COMPILE_FLAGS " -Wno-unused-but-set-variable")
            string(APPEND COMPILE_FLAGS " -Wno-maybe-uninitialized")
        endif()
    endif() # Clang

    if(WERROR)
        check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)
        if(NOT COMPILER_SUPPORT_WERROR)
            set(WERROR FALSE)
        else()
            string(APPEND COMPILE_FLAGS " -Werror")   
        endif()
    endif(WERROR)

# For MSVC 
else()
    string(APPEND CMAKE_C_FLAGS " /std:c11")
    # MSVC complains that this isn't a valid cmdline option.
    # Am I going wrong here, somehow?
    # string(APPEND CMAKE_CXX_FLAGS " /std:c++11")

    # Build in parallel
    string(APPEND COMPILE_FLAGS " /m:4")

    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND COMPILE_FLAGS " /O2")
    endif()

    string(APPEND COMPILE_FLAGS " /Wall")
    # string(APPEND COMPILE_FLAGS " /WX")  # Treats Linker Warnings as Errors
    
    # Ignore some MSVC warnings
    string(APPEND COMPILE_FLAGS " /wd4061")
    string(APPEND COMPILE_FLAGS " /wd4090")
    string(APPEND COMPILE_FLAGS " /wd4100")
    string(APPEND COMPILE_FLAGS " /wd4127")
    string(APPEND COMPILE_FLAGS " /wd4201")
    string(APPEND COMPILE_FLAGS " /wd4242")
    string(APPEND COMPILE_FLAGS " /wd4514")
    string(APPEND COMPILE_FLAGS " /wd4668")
    string(APPEND COMPILE_FLAGS " /wd4710")
    string(APPEND COMPILE_FLAGS " /wd4711")
    string(APPEND COMPILE_FLAGS " /wd4820")
    string(APPEND COMPILE_FLAGS " /wd4996")
    string(APPEND COMPILE_FLAGS " /wd5045")
    string(APPEND COMPILE_FLAGS " /wd5105")
endif()


# This library will be platform-independent, but still be installable following best practices. 
# We include GNUInstallDirs (included in CMake) which provides us with a set of variables containing installation 
# directories for various artifacts.
include(GNUInstallDirs)

# ------ Options ------
set(MUON_ROOT_DIR                    ${CMAKE_CURRENT_SOURCE_DIR})
set(MUON_BIN_DIR                     ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
set(MUON_LIB_DIR                     ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${MUON_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   ${MUON_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   ${MUON_LIB_DIR})

set(MUON_TARGET_NAME           ${PROJECT_NAME}Targets)
set(MUON_TARGETS_CMAKE         ${CMAKE_CURRENT_BINARY_DIR}/${MUON_TARGET_NAME}.cmake)

set(MUON_HEADERS_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/Muon)

# CMake Configurations (from the ``CMake`` folder)
set(MUON_CMAKE_CONFIGVERSION_IN      ${CMAKE_CURRENT_SOURCE_DIR}/CMake/ConfigVersion.cmake.in)
set(MUON_CMAKE_CONFIGVERSION         ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
set(MUON_CMAKE_CONFIG_IN             ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Config.cmake.in)
set(MUON_CMAKE_CONFIG                ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
set(MUON_CMAKE_CONFIGS_INSTALL_DIR   "lib/CMake/${PROJECT_NAME}" CACHE INTERNAL "")
set(MUON_CMAKE_PKGCONFIG_IN          ${CMAKE_CURRENT_SOURCE_DIR}/CMake/pkg-config.pc.in)
set(MUON_CMAKE_PKGCONFIG             ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Pkg-Config.pc)


# ------ Main Build ------
# Main Build File for the main Muon Library
# Note that this library must have _minimal_ dependencies 

# Where Muon's .h files can be found.
set(
    MUON_BUILD_INCLUDE_DIRS
    "${MUON_ROOT_DIR}/Muon"
    "${MUON_ROOT_DIR}"
)
include_directories(${MUON_BUILD_INCLUDE_DIRS})

# Create the Muon Interface Library
# Interfaces are best for header-only libraries
add_library(Muon INTERFACE)
# Add an alias so that Muon can be used with add_subdirectory
add_library(Muon::Muon ALIAS Muon)

if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    target_compile_features(Muon INTERFACE c_range_for)
else()
    target_compile_features(Muon INTERFACE c_std_11)
endif()

set_target_properties(Muon PROPERTIES VERSION ${MUON_VERSION})

target_include_directories(
    Muon 
    INTERFACE 
    # If Muon is used directly by another CMake target (such as when building tests or when it is included 
    # as a sub-directory)
    # $<BUILD_INTERFACE:"${MUON_HEADERS_INCLUDE_DIR}">
    "$<BUILD_INTERFACE:${MUON_BUILD_INCLUDE_DIRS}>"
    # The path if Muon is installed
    # $<INSTALL_INTERFACE:"include">
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}}>"
)


# ------ Installations ------
# Only perform the installation if Muon is built as the main project (i.e not included as an external project, 
# or a subdirectory) or destinations might break.
if(IS_MAIN_PROJECT)
    ## Installation header files, generate and install CMake config files for find_package()
    include(CMakePackageConfigHelpers)

    # We write a custom package version config file instead of `write_basic_package_version_file` to ensure that it's 
    # architecture-independent

    # ConfigVersion.CMake.in --> MuonConfigVersion.CMake
    configure_file(
        ${MUON_CMAKE_CONFIGVERSION_IN}
        ${MUON_CMAKE_CONFIGVERSION}
        @ONLY
    )
    # config.CMake.in --> MuonConfig.CMake
    configure_file(
        ${MUON_CMAKE_CONFIG_IN}
        ${MUON_CMAKE_CONFIG}
        @ONLY
    )
    # Install a pkg-config file, so other tools can find this.
    configure_file(
        ${MUON_CMAKE_PKGCONFIG_IN}
        ${MUON_CMAKE_PKGCONFIG}
    )
    # Install the pkg-config file to lib/pkgconfig
    install(
        FILES ${MUON_CMAKE_PKGCONFIG}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )

    # Move MuonConfig.CMake and MuonConfigVersion.CMake  ---> lib/CMake/Muon/...
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/MuonConfig.CMake" "${CMAKE_CURRENT_BINARY_DIR}/MuonConfigVersion.CMake" 
        DESTINATION "lib/CMake/Muon"
    )

    # ./include --> include
    install(
        DIRECTORY ${MUON_HEADERS_INCLUDE_DIR}
        DESTINATION Muon
    )


    # Export MuonTargets from MuonTargets.CMake
    export(
        TARGETS Muon
        FILE ${MUON_TARGETS_CMAKE}
    )

    install(
        EXPORT ${MUON_TARGET_NAME}
        DESTINATION lib/CMake/Muon
    )
endif() # IS_MAIN_PROJECT

# install(
#     TARGETS Muon
#     EXPORT "${MUON_TARGET_NAME}Targets"
#     INCLUDES DESTINATION ${MUON_HEADERS_INCLUDE_DIR}
# )
install(
    TARGETS Muon
    EXPORT ${MUON_TARGET_NAME}
    ARCHIVE DESTINATION "lib"
    LIBRARY DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    ## This results in an ``INTERFACE_INCLUDE_DIRECTORIES`` property contains path ... which is prefixed in the source 
    ## directory. 
    ## There may be something I'm missing out - if you know how to fix this, please send in a PR :)
    # INCLUDES DESTINATION ${MUON_HEADERS_INCLUDE_DIR)
)

# Header files are copied to the install directory
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Muon
    DESTINATION include
)

# If at least one test option is ON, we need to enter the `test` directory
# The relevant options will be parsed and the appropriate tests will be called
if(MUON_BUILDINTERNALTESTS OR MUON_BUILDTHIRDPARTYTESTS)
    add_subdirectory(test)
endif()