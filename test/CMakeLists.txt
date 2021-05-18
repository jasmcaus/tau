cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ARROW_TEST    "ArrowInternalTests")

add_executable(
    ${ARROW_TEST}
    test.c
)

# The Arrow INTERFACE library
target_link_libraries(${ARROW_TEST} ${PROJECT_NAME})