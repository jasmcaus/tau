#include <tau/tau.h>
#include "m6502.h"

struct M6502BranchTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502BranchTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502BranchTests){}

TEST_F( M6502BranchTests, BEQCanBranchForwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = true;
	tau->mem[0xFF00] = CPU::INS_BEQ;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQDoesNotBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = false;
	tau->mem[0xFF00] = CPU::INS_BEQ;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF02 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchForwardsIntoANewPageWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFEFD, tau->mem );
	tau->cpu.Flag.Z = true;
	tau->mem[0xFEFD] = CPU::INS_BEQ;
	tau->mem[0xFEFE] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF00 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFFCC, tau->mem );
	tau->cpu.Flag.Z = true;
	tau->mem[0xFFCC] = CPU::INS_BEQ;
	tau->mem[0xFFCD] = static_cast<Byte>( -0x2 );
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFFCC );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSetFromAssembleCode )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFFCC, tau->mem );
	tau->cpu.Flag.Z = true;
	/*
	loop
	lda #0
	beq loop
	*/
	tau->mem[0xFFCC] = 0xA9;
	tau->mem[0xFFCC+1] = 0x00;
	tau->mem[0xFFCC+2] = 0xF0;
	tau->mem[0xFFCC+3] = 0xFC;
	constexpr s32 EXPECTED_CYCLES = 2 + 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFFCC );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BNECanBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = false;
	tau->mem[0xFF00] = CPU::INS_BNE;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}


TEST_F( M6502BranchTests, BCSCanBranchForwardsWhenCarryFlagIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.C = true;
	tau->mem[0xFF00] = CPU::INS_BCS;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BCCCanBranchForwardsWhenCarryFlagIsNotSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.C = false;
	tau->mem[0xFF00] = CPU::INS_BCC;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BMICanBranchForwardsWhenNegativeFlagIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_BMI;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BPLCanBranchForwardsWhenCarryNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.N = false;
	tau->mem[0xFF00] = CPU::INS_BPL;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVSCanBranchForwardsWhenOverflowFlagIsSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.V = true;
	tau->mem[0xFF00] = CPU::INS_BVS;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVCCanBranchForwardsWhenOverflowNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.V = false;
	tau->mem[0xFF00] = CPU::INS_BVC;
	tau->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PC, 0xFF03 );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}