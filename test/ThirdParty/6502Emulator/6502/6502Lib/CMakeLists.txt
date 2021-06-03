cmake_minimum_required(VERSION 3.20)

project(TauThirdPartyTests_6502Lib)

set(TauThirdPartyTests_6502_SOURCES
    "src/public/m6502.h"
	"src/private/m6502.cpp"
    "src/private/main_6502.cpp"
)
		
source_group("src" FILES ${TauThirdPartyTests_6502_SOURCES})
		
add_library(TauThirdPartyTests_6502Lib ${TauThirdPartyTests_6502_SOURCES})

target_include_directories(
    TauThirdPartyTests_6502Lib 
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/public"
)
target_include_directories(
    TauThirdPartyTests_6502Lib 
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/private"
)