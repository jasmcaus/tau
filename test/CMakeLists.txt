cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MUON_TEST    "MuonInternalTests")


list(FIND CMAKE_C_COMPILE_FEATURES c_std_11 IDX)
if (${IDX} GREATER -1)
  if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
      set_source_files_properties(type_printers.c PROPERTIES
        COMPILE_FLAGS "-std=gnu11")

      set_source_files_properties(test.c PROPERTIES
        COMPILE_FLAGS "-Wall -Wextra -std=c11"
      )
  elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      set_source_files_properties(test.c PROPERTIES
        COMPILE_FLAGS "/Wall /wd4514 /wd5045"
      )
    else()
      set_source_files_properties(test.c PROPERTIES
        COMPILE_FLAGS "-Wall -Wextra -Weverything -std=c11"
      )
    endif()
  elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set_source_files_properties(test.c PROPERTIES
      COMPILE_FLAGS "/Wall /wd4514 /wd5045"
    )
  else()
    message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_source_files_properties(test.cpp PROPERTIES
    COMPILE_FLAGS "-Wall -Wextra -std=c++11"
  )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    set_source_files_properties(test.cpp PROPERTIES
      COMPILE_FLAGS "/Wall /wd4514 /wd5045"
    )
  else()
    set_source_files_properties(test.cpp PROPERTIES
      COMPILE_FLAGS "-Wall -Wextra -Weverything -std=c++11 -Wno-c++98-compat"
    )
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set_source_files_properties(test.cpp PROPERTIES
    COMPILE_FLAGS "/Wall /wd4514 /wd5045"
  )
else()
  message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
endif()

add_executable(
    ${MUON_TEST}
    # main.c 
    # test.c
    test.cpp
)

target_include_directories(
    ${MUON_TEST} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Muon INTERFACE library
target_link_libraries(${MUON_TEST} ${PROJECT_NAME})