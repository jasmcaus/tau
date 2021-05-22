cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MUON_TEST    "MuonInternalTests")

add_executable(
    ${MUON_TEST}
    main.c 
    test.c
    test.cpp
)

target_include_directories(
    ${MUON_TEST} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(
    ${MUON_TEST} 
    PRIVATE
    "/P \"F:/Documents/Dev/Github/Muon/compileroutput.txt\""
)

# The Muon INTERFACE library
target_link_libraries(${MUON_TEST} ${PROJECT_NAME})