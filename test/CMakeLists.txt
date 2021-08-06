cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# ------ Setting the C/CXX Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------ Internal Tests  ------
if(TAU_BUILDINTERNALTESTS)
    message("------- [INFO] Building Internal Tests")
    add_subdirectory(InternalTests)
endif() # TAU_BUILDINTERNALTESTS

# ------ Third-Party Tests  ------
if(TAU_BUILDTHIRDPARTYTESTS)
    message("------- [INFO] Building ThirdParty Tests")
    add_subdirectory(ThirdParty)
endif() # TAU_BUILDTHIRDPARTYTESTS