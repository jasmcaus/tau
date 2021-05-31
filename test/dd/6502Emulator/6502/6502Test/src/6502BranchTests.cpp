#include <Muon/Muon.h>
#include "m6502.h"

struct M6502BranchTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502BranchTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502BranchTests){}

TEST_F( M6502BranchTests, BEQCanBranchForwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->mem[0xFF00] = CPU::INS_BEQ;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQDoesNotBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = false;
	muon->mem[0xFF00] = CPU::INS_BEQ;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF02 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchForwardsIntoANewPageWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFEFD, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->mem[0xFEFD] = CPU::INS_BEQ;
	muon->mem[0xFEFE] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF00 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFFCC, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->mem[0xFFCC] = CPU::INS_BEQ;
	muon->mem[0xFFCD] = static_cast<Byte>( -0x2 );
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFFCC );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSetFromAssembleCode )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFFCC, muon->mem );
	muon->cpu.Flag.Z = true;
	/*
	loop
	lda #0
	beq loop
	*/
	muon->mem[0xFFCC] = 0xA9;
	muon->mem[0xFFCC+1] = 0x00;
	muon->mem[0xFFCC+2] = 0xF0;
	muon->mem[0xFFCC+3] = 0xFC;
	constexpr s32 EXPECTED_CYCLES = 2 + 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFFCC );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BNECanBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = false;
	muon->mem[0xFF00] = CPU::INS_BNE;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}


TEST_F( M6502BranchTests, BCSCanBranchForwardsWhenCarryFlagIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->mem[0xFF00] = CPU::INS_BCS;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BCCCanBranchForwardsWhenCarryFlagIsNotSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->mem[0xFF00] = CPU::INS_BCC;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BMICanBranchForwardsWhenNegativeFlagIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_BMI;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BPLCanBranchForwardsWhenCarryNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_BPL;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVSCanBranchForwardsWhenOverflowFlagIsSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.V = true;
	muon->mem[0xFF00] = CPU::INS_BVS;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVCCanBranchForwardsWhenOverflowNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.V = false;
	muon->mem[0xFF00] = CPU::INS_BVC;
	muon->mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PC, 0xFF03 );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}