cmake_minimum_required(VERSION 3.7)

project( M6502Lib )

set  (M6502_SOURCES
    "src/public/m6502.h"
	"src/private/m6502.cpp"
    "src/private/main_6502.cpp"
)
		
source_group("src" FILES ${M6502_SOURCES})
		
add_library( M6502Lib ${M6502_SOURCES} )

target_include_directories ( M6502Lib PUBLIC "${PROJECT_SOURCE_DIR}/src/public")
target_include_directories ( M6502Lib PRIVATE "${PROJECT_SOURCE_DIR}/src/private")