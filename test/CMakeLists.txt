cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MUON_TEST    "MuonInternalTests")

# # -------------- C Source File Properties
# if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
#     set_source_files_properties(
#         test.c 
#         PROPERTIES
#         COMPILE_FLAGS "-Wall -Wextra -Wno-multistatement-macros -std=c11"
#     )
# elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
#     if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
#         set_source_files_properties(
#             test.c 
#             PROPERTIES
#             COMPILE_FLAGS "/Wall /O2 /std:c11 /wd4061 /wd4100 /wd4127 /wd4514 /wd4710 /wd4711 /wd5045 /wd5105"
#         )
#     else()
#         set_source_files_properties(
#             test.c 
#             PROPERTIES
#             COMPILE_FLAGS "-Wall -Wextra -Werror -std=c11"
#         )
#     endif()
# elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
#     set_source_files_properties(
#         test.c 
#         PROPERTIES
#         COMPILE_FLAGS "/Wall /O2 /std:c11 /wd4061 /wd4100 /wd4127 /wd4514 /wd4710 /wd4711 /wd5045 /wd5105"
#     )
# else()
#     message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
# endif()

# # -------------- C++ Source File Properties
# if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#     set_source_files_properties(
#         test.cpp 
#         PROPERTIES
#         COMPILE_FLAGS "-Wall -Wextra -Wno-multistatement-macros -std=c++11"
#     )
# elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#     if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
#         set_source_files_properties(
#             test.cpp 
#             PROPERTIES
#             COMPILE_FLAGS "/Wall /O2 /std:c11 /wd4061 /wd4100 /wd4127 /wd4514 /wd4710 /wd4711 /wd5045 /wd5105"
#         )
#     else()
#         set_source_files_properties(
#             test.cpp 
#             PROPERTIES
#             COMPILE_FLAGS "-Wall -Wextra -Wno-multistatement-macros -std=c++11"
#         )
#     endif()
# elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#     set_source_files_properties(
#         test.cpp 
#         PROPERTIES
#         COMPILE_FLAGS "/Wall /O2 /std:c11 /wd4061 /wd4100 /wd4127 /wd4514 /wd4710 /wd4711 /wd5045 /wd5105"
#     )
# else()
#     message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
# endif()

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

# if(CMAKE_VERSION VERSION_LESS 3.8)
#     set_property(
#         ${MUON_TEST} 
#         PROPERTY 
#         C_STANDARD 11
#         CXX_STANDARD 11
#     )
# else()
#     target_compile_features(
#         ${MUON_TEST}
#         PRIVATE
#         c_std_11
#         cxx_std_11
#     )
# endif()

# The Muon INTERFACE library
target_link_libraries(${MUON_TEST} ${PROJECT_NAME})