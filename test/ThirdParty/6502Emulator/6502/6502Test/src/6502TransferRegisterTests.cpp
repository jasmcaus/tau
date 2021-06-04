#include <tau/tau.h>
#include "m6502.h"


struct M6502TransferRegistgerTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	void ExpectUnaffectedRegisters( m6502::CPU CPUBefore )
	{
		CHECK_EQ( CPUBefore.Flag.C, cpu.Flag.C );
		CHECK_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		CHECK_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		CHECK_EQ( CPUBefore.Flag.B, cpu.Flag.B );
		CHECK_EQ( CPUBefore.Flag.V, cpu.Flag.V );
	}
};

TEST_F_SETUP(M6502TransferRegistgerTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502TransferRegistgerTests){}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x42;
	tau->cpu.X = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_EQ( tau->cpu.X, 0x42 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x0;
	tau->cpu.X = 0x32;
	tau->cpu.Flag.Z = false;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x0 );
	CHECK_EQ( tau->cpu.X, 0x0 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0b10001011;
	tau->cpu.X = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = false;
	tau->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0b10001011 );
	CHECK_EQ( tau->cpu.X, 0b10001011 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x42;
	tau->cpu.Y = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_EQ( tau->cpu.Y, 0x42 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0x0;
	tau->cpu.Y = 0x32;
	tau->cpu.Flag.Z = false;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x0 );
	CHECK_EQ( tau->cpu.Y, 0x0 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.A = 0b10001011;
	tau->cpu.Y = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = false;
	tau->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0b10001011 );
	CHECK_EQ( tau->cpu.Y, 0b10001011 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.X = 0x42;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0x42 );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.X = 0x0;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = false;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0x0 );
	CHECK_EQ( tau->cpu.A, 0x0 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.X = 0b10001011;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = false;
	tau->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.X, 0b10001011 );
	CHECK_EQ( tau->cpu.A, 0b10001011 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Y = 0x42;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.Y, 0x42 );
	CHECK_EQ( tau->cpu.A, 0x42 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Y = 0x0;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = false;
	tau->cpu.Flag.N = true;
	tau->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.Y, 0x0 );
	CHECK_EQ( tau->cpu.A, 0x0 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Y = 0b10001011;
	tau->cpu.A = 0x32;
	tau->cpu.Flag.Z = true;
	tau->cpu.Flag.N = false;
	tau->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.Y, 0b10001011 );
	CHECK_EQ( tau->cpu.A, 0b10001011 );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.N );
	tau->ExpectUnaffectedRegisters( CPUCopy );
}