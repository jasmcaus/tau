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
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502TransferRegistgerTests){}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x42;
	muon->cpu.X = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_EQ( muon->cpu.X, 0x42 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x0;
	muon->cpu.X = 0x32;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x0 );
	CHECK_EQ( muon->cpu.X, 0x0 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAXCanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0b10001011;
	muon->cpu.X = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_TAX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10001011 );
	CHECK_EQ( muon->cpu.X, 0b10001011 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x42;
	muon->cpu.Y = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_EQ( muon->cpu.Y, 0x42 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0x0;
	muon->cpu.Y = 0x32;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0x0 );
	CHECK_EQ( muon->cpu.Y, 0x0 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TAYCanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.A = 0b10001011;
	muon->cpu.Y = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_TAY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.A, 0b10001011 );
	CHECK_EQ( muon->cpu.Y, 0b10001011 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0x42;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x42 );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0x0;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x0 );
	CHECK_EQ( muon->cpu.A, 0x0 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TXACanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0b10001011;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_TXA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0b10001011 );
	CHECK_EQ( muon->cpu.A, 0b10001011 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANonNegativeNonZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0x42;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0x42 );
	CHECK_EQ( muon->cpu.A, 0x42 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANonNegativeZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0x0;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0x0 );
	CHECK_EQ( muon->cpu.A, 0x0 );
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502TransferRegistgerTests, TYACanTransferANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0b10001011;
	muon->cpu.A = 0x32;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_TYA;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0b10001011 );
	CHECK_EQ( muon->cpu.A, 0b10001011 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedRegisters( CPUCopy );
}