cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_executable(
    ArrowTest
    ${CMAKE_SOURCE_DIR}/include/Arrow/Types.h
    ${CMAKE_SOURCE_DIR}/include/Arrow/Arrow.h
    test.c
)

target_include_directories(ArrowTest PRIVATE ${CMAKE_BINARY_DIR}/include)
# The Arrow INTERFACE library
target_link_libraries(ArrowTest ${ARROW_TARGET_NAME})