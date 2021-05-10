cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_executable(
    ArrowTest
    ../Arrow/Types.h
    ../Arrow/Arrow.h
    test.c
)

target_include_directories(
    ArrowTest 
    PRIVATE 
    # ${CMAKE_SOURCE_DIR}           # For other users that link to Arrow 
    ${CMAKE_CURRENT_SOURCE_DIR}/../ # Target the root of Arrow
    ${CMAKE_BINARY_DIR}/Arrow       # Default Bin location
)
# The Arrow INTERFACE library
target_link_libraries(ArrowTest ${ARROW_TARGET_NAME})