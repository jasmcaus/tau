cmake_minimum_required(VERSION 3.7)

project(MUON_ThirdPartyTests_M6502Lib)

set(MUON_ThirdPartyTests_M6502_SOURCES
    "src/public/m6502.h"
	"src/private/m6502.cpp"
    "src/private/main_6502.cpp"
)
		
source_group("src" FILES ${MUON_ThirdPartyTests_M6502_SOURCES})
		
add_library(MUON_ThirdPartyTests_M6502Lib ${MUON_ThirdPartyTests_M6502_SOURCES})

target_include_directories(
    MUON_ThirdPartyTests_M6502Lib 
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/public"
)
target_include_directories(
    MUON_ThirdPartyTests_M6502Lib 
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/private"
)