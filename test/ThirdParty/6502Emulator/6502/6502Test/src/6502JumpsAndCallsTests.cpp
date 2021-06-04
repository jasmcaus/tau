#include <tau/tau.h>
#include "m6502.h"

struct M6502JumpsAndCallsTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502JumpsAndCallsTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502JumpsAndCallsTests){}

TEST_F( M6502JumpsAndCallsTests, CanJumpToASubroutineAndJumpBackAgain )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_JSR;
	tau->mem[0xFF01] = 0x00;
	tau->mem[0xFF02] = 0x80;
	tau->mem[0x8000] = CPU::INS_RTS;
	tau->mem[0xFF03] = CPU::INS_LDA_IM;
	tau->mem[0xFF04] = 0x42;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_EQ( tau->cpu.SP, CPUCopy.SP );
}

TEST_F( M6502JumpsAndCallsTests, JSRDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_JSR;
	tau->mem[0xFF01] = 0x00;
	tau->mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_NE( tau->cpu.SP, CPUCopy.SP );
	CHECK_EQ( tau->cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, RTSDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_JSR;
	tau->mem[0xFF01] = 0x00;
	tau->mem[0xFF02] = 0x80;
	tau->mem[0x8000] = CPU::INS_RTS;
	constexpr s32 EXPECTED_CYCLES = 6 + 6;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
}

TEST_F( M6502JumpsAndCallsTests, JumpAbsoluteCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_JMP_ABS;
	tau->mem[0xFF01] = 0x00;
	tau->mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.SP, CPUCopy.SP );
	CHECK_EQ( tau->cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, JumpIndirectCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_JMP_IND;
	tau->mem[0xFF01] = 0x00;
	tau->mem[0xFF02] = 0x80;
	tau->mem[0x8000] = 0x00;
	tau->mem[0x8001] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.SP, CPUCopy.SP );
	CHECK_EQ( tau->cpu.PC, 0x9000 );
}
