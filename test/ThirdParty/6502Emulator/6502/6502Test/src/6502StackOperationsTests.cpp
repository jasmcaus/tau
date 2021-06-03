#include <tau/tau.h>
#include "m6502.h"

struct M6502StackOperationsTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502StackOperationsTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502StackOperationsTests){}

TEST_F( M6502StackOperationsTests, TSXCanTransferTheStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = muon->cpu.Flag.N = true;
	muon->cpu.X = 0x00;
	muon->cpu.SP = 0x01;
	muon->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x01 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TSXCanTransferAZeroStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = muon->cpu.Flag.N = true;
	muon->cpu.X = 0x00;
	muon->cpu.SP = 0x00;
	muon->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x00 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TSXCanTransferANegativeStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = muon->cpu.Flag.N = false;
	muon->cpu.X = 0x00;
	muon->cpu.SP = 0b10000000;
	muon->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0b10000000 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TXSCanTransferXRegisterToTheStackPointer )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0xFF;
	muon->cpu.SP = 0;
	muon->mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.SP, 0xFF );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502StackOperationsTests, PHACanPushARegsiterOntoTheStack )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x42;
	muon->mem[0xFF00] = CPU::INS_PHA;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[muon->cpu.SPToAddress()+1] ,muon->cpu.A );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_EQ( muon->cpu.SP, 0xFE );
}

TEST_F( M6502StackOperationsTests, PLACanPullAValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x00;
	muon->cpu.SP = 0xFE;
	muon->mem[0x01FF] = 0x42;
	muon->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_EQ( muon->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PLACanPullAZeroValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->cpu.A = 0x42;
	muon->cpu.SP = 0xFE;
	muon->mem[0x01FF] = 0x00;
	muon->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x00 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	CHECK_EQ( muon->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PLACanPullANegativeValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.N = false;
	muon->cpu.Flag.Z = true;
	muon->cpu.A = 0x42;
	muon->cpu.SP = 0xFE;
	muon->mem[0x01FF] = 0b10000001;
	muon->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10000001 );
	CHECK_TRUE( muon->cpu.Flag.N );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_EQ( muon->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PHPCanPushProcessorStatusOntoTheStack )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.PS = 0xCC;
	muon->mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[muon->cpu.SPToAddress() + 1], 
		0xCC | CPU::UnusedFlagBit | CPU::BreakFlagBit );
	CHECK_EQ( muon->cpu.PS, CPUCopy.PS );
	CHECK_EQ( muon->cpu.SP, 0xFE );
}

TEST_F( M6502StackOperationsTests, PHPAlwaysSetsBits4And5OnTheStack )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.PS = 0x0;
	muon->mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	Word AddPSOnStack = muon->cpu.SPToAddress() + 1;
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );

	// https://wiki.nesdev.com/w/index.php/Status_flags
	//Two interrupts (/IRQ and /NMI) and two instructions (PHP and BRK) push 
	// the flags to the stack. In the byte pushed, bit 5 is always set to 1, 
	//and bit 4 is 1 if from an instruction (PHP or BRK) or 0 if from an 
	// interrupt line being pulled low (/IRQ or /NMI). This is the only time 
	// and place where the B flag actually exists: not in the status register 
	// itself, but in bit 4 of the copy that is written to the stack. 
	const Byte FlagsOnStack = CPU::UnusedFlagBit | CPU::BreakFlagBit;
	CHECK_EQ( muon->mem[AddPSOnStack], FlagsOnStack );
}

TEST_F( M6502StackOperationsTests, PLPCanPullAValueFromTheStackIntoTheProcessorStatus )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.SP = 0xFE;
	muon->cpu.PS = 0;
	muon->mem[0x01FF] = 0x42 | CPU::BreakFlagBit | CPU::UnusedFlagBit;
	muon->mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, 0x42 );
}

TEST_F( M6502StackOperationsTests, PLPClearsBits4And5WhenPullingFromTheStack )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.SP = 0xFE;
	muon->cpu.PS = 0;
	muon->mem[0x01FF] = CPU::BreakFlagBit | CPU::UnusedFlagBit;
	muon->mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.PS, 0 );
}