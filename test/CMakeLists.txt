cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ARROW_TEST    "ArrowTest")

add_executable(
    ${ARROW_TEST}
    ../Arrow/Types.h
    ../Arrow/Arrow.h
    test.c
)

target_include_directories(
    ${ARROW_TEST} 
    PRIVATE 
    # ${CMAKE_SOURCE_DIR}           # For other users that link to Arrow 
    ${CMAKE_CURRENT_SOURCE_DIR}/../ # Target the root of Arrow
    ${CMAKE_BINARY_DIR}/Arrow       # Default Bin location
)
# The Arrow INTERFACE library
target_link_libraries(${ARROW_TEST} ${PACKAGE_NAME})