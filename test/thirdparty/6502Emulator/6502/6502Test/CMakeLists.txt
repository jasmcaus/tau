cmake_minimum_required(VERSION 3.25)

project( MuonThirdPartyTests_6502Emulator )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DM6502_DEBUG")	
#so we can add the DEBUG preprocessor define and other flags to stay in debug mode - see https://cmake.org/Wiki/CMake_Useful_Variables#Compilers_and_Tools
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DM6502_DEBUG")


# source for the test executable
set (MuonThirdPartyTests_6502_SOURCES
	"src/main_6502.cpp"
	"src/6502LoadRegisterTests.cpp"
	"src/6502StoreRegisterTests.cpp"
	"src/6502JumpsAndCallsTests.cpp"
	"src/6502StackOperationsTests.cpp"
	"src/6502AND_EOR_ORA_Tests.cpp"
	"src/6502LoadPrgTests.cpp"
	"src/6502TransferRegisterTests.cpp"
	"src/6502IncrementDecrementTests.cpp"
	"src/6502BranchTests.cpp"
	"src/6502StatusFlagChangeTests.cpp"
	"src/6502AddWithCarryTests.cpp"
	"src/6502CompareRegisterTests.cpp"
	"src/6502ShiftsTests.cpp"
	"src/6502SystemFunctionsTests.cpp"
)
		
source_group("src" FILES ${MuonThirdPartyTests_6502_SOURCES})
		
add_executable(MuonThirdPartyTests_6502 ${MuonThirdPartyTests_6502_SOURCES})
add_dependencies( MuonThirdPartyTests_6502 MuonThirdPartyTests_6502Lib )

install(
	TARGETS MuonThirdPartyTests_6502
	RUNTIME DESTINATION ${MUON_BIN_DIR}
)

target_link_libraries(MuonThirdPartyTests_6502 MuonThirdPartyTests_6502Lib)
target_link_libraries(MuonThirdPartyTests_6502 Muon)

