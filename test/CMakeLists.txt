cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MUON_TEST    "MuonInternalTests")

add_executable(
    ${MUON_TEST}
    # main.c 
    test.c
    # test.cpp
)

# The Muon INTERFACE library
target_link_libraries(${MUON_TEST} ${PROJECT_NAME})