cmake_minimum_required(VERSION 3.20)

project( DavePoo6502Emulator LANGUAGES CXX )

if(NOT MSVC)
    string(APPEND COMPILE_FLAGS " -Wno-switch")
endif()

add_subdirectory(6502/6502Test)
add_subdirectory(6502/6502Lib)