#include <Muon/Muon.h>
#include "m6502.h"

struct M6502JumpsAndCallsTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502JumpsAndCallsTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502JumpsAndCallsTests){}

TEST_F( M6502JumpsAndCallsTests, CanJumpToASubroutineAndJumpBackAgain )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->mem[0xFF00] = CPU::INS_JSR;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = CPU::INS_RTS;
	muon->mem[0xFF03] = CPU::INS_LDA_IM;
	muon->mem[0xFF04] = 0x42;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_EQ( muon->cpu.SP, CPUCopy.SP );
}

TEST_F( M6502JumpsAndCallsTests, JSRDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->mem[0xFF00] = CPU::INS_JSR;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_NE( muon->cpu.SP, CPUCopy.SP );
	CHECK_EQ( muon->cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, RTSDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->mem[0xFF00] = CPU::INS_JSR;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = CPU::INS_RTS;
	constexpr s32 EXPECTED_CYCLES = 6 + 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
}

TEST_F( M6502JumpsAndCallsTests, JumpAbsoluteCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->mem[0xFF00] = CPU::INS_JMP_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_EQ( muon->cpu.SP, CPUCopy.SP );
	CHECK_EQ( muon->cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, JumpIndirectCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->mem[0xFF00] = CPU::INS_JMP_IND;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0x00;
	muon->mem[0x8001] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_EQ( muon->cpu.SP, CPUCopy.SP );
	CHECK_EQ( muon->cpu.PC, 0x9000 );
}
