#include <tau/tau.h>
#include "m6502.h"

struct M6502StackOperationsTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502StackOperationsTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502StackOperationsTests){}

TEST_F( M6502StackOperationsTests, TSXCanTransferTheStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = tau->cpu.Flag.N = true;
	tau->cpu.X = 0x00;
	tau->cpu.SP = 0x01;
	tau->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0x01 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TSXCanTransferAZeroStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = tau->cpu.Flag.N = true;
	tau->cpu.X = 0x00;
	tau->cpu.SP = 0x00;
	tau->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0x00 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TSXCanTransferANegativeStackPointerToXRegister )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = tau->cpu.Flag.N = false;
	tau->cpu.X = 0x00;
	tau->cpu.SP = 0b10000000;
	tau->mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0b10000000 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.N );
}

TEST_F( M6502StackOperationsTests, TXSCanTransferXRegisterToTheStackPointer )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.X = 0xFF;
	tau->cpu.SP = 0;
	tau->mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.SP, 0xFF );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
}

TEST_F( M6502StackOperationsTests, PHACanPushARegsiterOntoTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x42;
	tau->mem[0xFF00] = CPU::INS_PHA;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[tau->cpu.SPToAddress()+1] ,tau->cpu.A );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.SP, 0xFE );
}

TEST_F( M6502StackOperationsTests, PLACanPullAValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x00;
	tau->cpu.SP = 0xFE;
	tau->mem[0x01FF] = 0x42;
	tau->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_EQ( tau->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PLACanPullAZeroValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.Z = false;
	tau->cpu.Flag.N = true;
	tau->cpu.A = 0x42;
	tau->cpu.SP = 0xFE;
	tau->mem[0x01FF] = 0x00;
	tau->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x00 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	CHECK_EQ( tau->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PLACanPullANegativeValueFromTheStackIntoTheARegsiter )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.N = false;
	tau->cpu.Flag.Z = true;
	tau->cpu.A = 0x42;
	tau->cpu.SP = 0xFE;
	tau->mem[0x01FF] = 0b10000001;
	tau->mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0b10000001 );
	CHECK_TRUE( tau->cpu.Flag.N );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_EQ( tau->cpu.SP, 0xFF );
}

TEST_F( M6502StackOperationsTests, PHPCanPushProcessorStatusOntoTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.PS = 0xCC;
	tau->mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[tau->cpu.SPToAddress() + 1], 
		0xCC | CPU::UnusedFlagBit | CPU::BreakFlagBit );
	CHECK_EQ( tau->cpu.PS, CPUCopy.PS );
	CHECK_EQ( tau->cpu.SP, 0xFE );
}

TEST_F( M6502StackOperationsTests, PHPAlwaysSetsBits4And5OnTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.PS = 0x0;
	tau->mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	Word AddPSOnStack = tau->cpu.SPToAddress() + 1;
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );

	// https://wiki.nesdev.com/w/index.php/Status_flags
	//Two interrupts (/IRQ and /NMI) and two instructions (PHP and BRK) push 
	// the flags to the stack. In the byte pushed, bit 5 is always set to 1, 
	//and bit 4 is 1 if from an instruction (PHP or BRK) or 0 if from an 
	// interrupt line being pulled low (/IRQ or /NMI). This is the only time 
	// and place where the B flag actually exists: not in the status register 
	// itself, but in bit 4 of the copy that is written to the stack. 
	const Byte FlagsOnStack = CPU::UnusedFlagBit | CPU::BreakFlagBit;
	CHECK_EQ( tau->mem[AddPSOnStack], FlagsOnStack );
}

TEST_F( M6502StackOperationsTests, PLPCanPullAValueFromTheStackIntoTheProcessorStatus )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.SP = 0xFE;
	tau->cpu.PS = 0;
	tau->mem[0x01FF] = 0x42 | CPU::BreakFlagBit | CPU::UnusedFlagBit;
	tau->mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, 0x42 );
}

TEST_F( M6502StackOperationsTests, PLPClearsBits4And5WhenPullingFromTheStack )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.SP = 0xFE;
	tau->cpu.PS = 0;
	tau->mem[0x01FF] = CPU::BreakFlagBit | CPU::UnusedFlagBit;
	tau->mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.PS, 0 );
}