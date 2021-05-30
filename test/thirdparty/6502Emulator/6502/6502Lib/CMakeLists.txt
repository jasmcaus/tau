cmake_minimum_required(VERSION 3.7)

project(MuonThirdPartyTests_6502Lib)

set(MuonThirdPartyTests_6502_SOURCES
    "src/public/m6502.h"
	"src/private/m6502.cpp"
    "src/private/main_6502.cpp"
)
		
source_group("src" FILES ${MuonThirdPartyTests_6502_SOURCES})
		
add_library(MuonThirdPartyTests_6502Lib ${MuonThirdPartyTests_6502_SOURCES})

target_include_directories(
    MuonThirdPartyTests_6502Lib 
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/public"
)
target_include_directories(
    MuonThirdPartyTests_6502Lib 
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/private"
)