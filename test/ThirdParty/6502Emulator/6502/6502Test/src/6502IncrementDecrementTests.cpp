#include <tau/tau.h>
#include "m6502.h"

struct M6502IncrementDecrementTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	void ExpectUnaffectedFlags( m6502::CPU CPUBefore )
	{
		CHECK_EQ( CPUBefore.Flag.C, cpu.Flag.C );
		CHECK_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		CHECK_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		CHECK_EQ( CPUBefore.Flag.B, cpu.Flag.B );
		CHECK_EQ( CPUBefore.Flag.V, cpu.Flag.V );
	}
};

TEST_F_SETUP(M6502IncrementDecrementTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502IncrementDecrementTests){}

TEST_F( M6502IncrementDecrementTests, INXCanIncrementAZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0x0;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x1 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INXCanIncrement255 )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0xFF;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0x0 );	//NOTE: does this instruction actually wrap?
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INXCanIncrementANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0b10001000;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0b10001001 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}


TEST_F( M6502IncrementDecrementTests, INYCanIncrementAZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0x0;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0x1 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INYCanIncrement255 )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0xFF;
	muon->cpu.Flag.Z = false;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0x0 );	//NOTE: does this instruction actually wrap?
	CHECK_TRUE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INYCanIncrementANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0b10001000;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0b10001001 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}


TEST_F( M6502IncrementDecrementTests, DEYCanDecementAZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0x0;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0xFF );	//NOTE: Does this wrap?
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEYCanDecrement255 )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0xFF;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0xFE );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEYCanDecrementANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Y = 0b10001001;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.Y, 0b10001000 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecementAZeroValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0x0;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0xFF );	//NOTE: Does this wrap?
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecrement255 )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0xFF;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0xFE );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecrementANegativeValue )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.X = 0b10001001;
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = false;
	muon->mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->cpu.X, 0b10001000 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_TRUE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueInTheZeroPage )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_DEC_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0x56 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueInTheZeroPageX )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_DEC_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0x56 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueAbsolute )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_DEC_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0x56 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueAbsoluteX )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_DEC_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0x56 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueInTheZeroPage )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INC_ZP;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042], 0x58 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueInTheZeroPageX )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_INC_ZPX;
	muon->mem[0xFF01] = 0x42;
	muon->mem[0x0042 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x0042 + 0x10], 0x58 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueAbsolute )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->mem[0xFF00] = CPU::INS_INC_ABS;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000], 0x58 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueAbsoluteX )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.Z = true;
	muon->cpu.Flag.N = true;
	muon->cpu.X = 0x10;
	muon->mem[0xFF00] = CPU::INS_INC_ABSX;
	muon->mem[0xFF01] = 0x00;
	muon->mem[0xFF02] = 0x80;
	muon->mem[0x8000 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( muon->mem[0x8000 + 0x10], 0x58 );
	CHECK_FALSE( muon->cpu.Flag.Z );
	CHECK_FALSE( muon->cpu.Flag.N );
	muon->ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, TestLoadAProgramThatCanIncMemory )
{
	// given:
	using namespace m6502;

	// when:
	/*
	* = $1000

	lda #00
	sta $42

	start
	inc $42
	ldx $42
	inx
	jmp start
	*/
	Byte TestPrg[] = 
		{ 0x0,0x10,0xA9,0x00,0x85,0x42,0xE6,0x42,
		0xA6,0x42,0xE8,0x4C,0x04,0x10 };

	Word StartAddress = muon->cpu.LoadPrg( TestPrg, sizeof(TestPrg), muon->mem );
	muon->cpu.PC = StartAddress;

	//then:
	for ( m6502::s32 Clock = 1000; Clock > 0; )
	{
		Clock -= muon->cpu.Execute( 1, muon->mem );
	}
}