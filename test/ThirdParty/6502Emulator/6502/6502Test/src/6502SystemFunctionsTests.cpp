#include <tau/tau.h>
#include "m6502.h"

struct M6502SystemFunctionsTests {
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502SystemFunctionsTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502SystemFunctionsTests){}

TEST_F( M6502SystemFunctionsTests, NOPWillDoNothingButConsumeACycle )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_NOP;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.PC, 0xFF01 );
	CHECK_EQ( tau->cpu.A, CPUCopy.A );
	CHECK_EQ( tau->cpu.X, CPUCopy.X );
	CHECK_EQ( tau->cpu.Y, CPUCopy.Y );
	CHECK_EQ( tau->cpu.SP, CPUCopy.SP );
}

TEST_F( M6502SystemFunctionsTests, BRKWillLoadTheProgramCounterFromTheInterruptVector )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_BRK;
	tau->mem[0xFFFE] = 0x00;
	tau->mem[0xFFFF] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0x8000 );
}

TEST_F( M6502SystemFunctionsTests, BRKWillLoadTheProgramCounterFromTheInterruptVector2 )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_BRK;
	tau->mem[0xFFFE] = 0x00;
	tau->mem[0xFFFF] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0x9000 );
}

TEST_F( M6502SystemFunctionsTests, BRKWillSetTheBreakFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.B = false;
	tau->mem[0xFF00] = CPU::INS_BRK;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( tau->cpu.Flag.B );
}

TEST_F( M6502SystemFunctionsTests, BRKWillPush3BytesOntoTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_BRK;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.SP, CPUCopy.SP - 3 );
}

TEST_F( M6502SystemFunctionsTests, BRKWillPushPCandPSOntoTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_BRK;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = tau->cpu;
	Byte OldSP = CPUCopy.SP;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[(0x100 | OldSP)-0], 0xFF );
	// https://www.c64-wiki.com/wiki/BRK
	// Note that since BRK increments the program counter by 
	// 2 instead of 1, it is advisable to use a NOP after it 
	// to avoid issues
	CHECK_EQ( tau->mem[(0x100 | OldSP)-1], 0x02 );
	CHECK_EQ( tau->mem[(0x100 | OldSP)-2], 
		CPUCopy.PS 
		| CPU::UnusedFlagBit 
		| CPU::BreakFlagBit );

	// https://wiki.nesdev.com/w/index.php/Status_flags
	// Instruction	|Bits 5 and 4	| Side effects after pushing 
	// BRK			|	11			| I is set to 1 
	CHECK_EQ( tau->cpu.Flag.I, true );
}

TEST_F( M6502SystemFunctionsTests, RTICanReturnFromAnInterruptLeavingTheCPUInTheStateWhenItEntered )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->mem[0xFF00] = CPU::INS_BRK;
	tau->mem[0xFFFE] = 0x00;
	tau->mem[0xFFFF] = 0x80;
	tau->mem[0x8000] = CPU::INS_RTI;
	constexpr s32 EXPECTED_CYCLES_BRK = 7;
	constexpr s32 EXPECTED_CYCLES_RTI = 6;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCyclesBRK = tau->cpu.Execute( EXPECTED_CYCLES_BRK, tau->mem );
	const s32 ActualCyclesRTI = tau->cpu.Execute( EXPECTED_CYCLES_RTI, tau->mem );

	// then:
	CHECK_EQ( ActualCyclesBRK, EXPECTED_CYCLES_BRK );
	CHECK_EQ( ActualCyclesRTI, EXPECTED_CYCLES_RTI );
	CHECK_EQ( CPUCopy.SP, tau->cpu.SP );
	CHECK_EQ( 0xFF02, tau->cpu.PC );
	CHECK_EQ( CPUCopy.PS, tau->cpu.PS );
}


