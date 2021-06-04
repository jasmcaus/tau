#include <tau/tau.h>
#include "m6502.h"

static void VerfifyUnmodifiedFlagsFromStoreRegister(
	const m6502::CPU& cpu,
	const m6502::CPU& CPUCopy )
{
	CHECK_EQ( cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( cpu.Flag.N, CPUCopy.Flag.N );
}

struct M6502StoreRegisterTests {
public:	
	m6502::Mem mem;
	m6502::CPU cpu;

	void TestStoreRegisterZeroPage(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*Register ) {
		// given:
		using namespace m6502;
		cpu.*Register = 0x2F;
		mem[0xFFFC] = OpcodeToTest;
		mem[0xFFFD] = 0x80;
		mem[0x0080] = 0x00;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( mem[0x0080], 0x2F );
		VerfifyUnmodifiedFlagsFromStoreRegister( cpu, CPUCopy );
	}

	void TestStoreRegisterAbsolute(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*Register )
	{
		// given:
		using namespace m6502;
		cpu.*Register = 0x2F;
		mem[0xFFFC] = OpcodeToTest;
		mem[0xFFFD] = 0x00;
		mem[0xFFFE] = 0x80;
		mem[0x8000] = 0x00;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( mem[0x8000], 0x2F );
		VerfifyUnmodifiedFlagsFromStoreRegister( cpu, CPUCopy );
	}

	void TestStoreRegisterZeroPageX(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*Register )
	{
		// given:
		using namespace m6502;
		cpu.*Register = 0x42;
		cpu.X = 0x0F;
		mem[0xFFFC] = OpcodeToTest;
		mem[0xFFFD] = 0x80;
		mem[0x008F] = 0x00;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( mem[0x008F], 0x42 );
		VerfifyUnmodifiedFlagsFromStoreRegister( cpu, CPUCopy );
	}

	void TestStoreRegisterZeroPageY(
		m6502::Byte OpcodeToTest,
		m6502::Byte m6502::CPU::*Register )
	{
		// given:
		using namespace m6502;
		cpu.*Register = 0x42;
		cpu.Y = 0x0F;
		mem[0xFFFC] = OpcodeToTest;
		mem[0xFFFD] = 0x80;
		mem[0x008F] = 0x00;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( mem[0x008F], 0x42 );
		VerfifyUnmodifiedFlagsFromStoreRegister( cpu, CPUCopy );
	}
};


TEST_F_SETUP(M6502StoreRegisterTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502StoreRegisterTests){}


TEST_F( M6502StoreRegisterTests, STAZeroPageCanStoreTheARegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPage( CPU::INS_STA_ZP, &CPU::A );
}

TEST_F( M6502StoreRegisterTests, STXZeroPageCanStoreTheXRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPage( CPU::INS_STX_ZP, &CPU::X );
}

TEST_F( M6502StoreRegisterTests, STXZeroPageYCanStoreTheXRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPageY( CPU::INS_STX_ZPY, &CPU::X );
}

TEST_F( M6502StoreRegisterTests, STYZeroPageCanStoreTheYRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPage( CPU::INS_STY_ZP, &CPU::Y );
}

TEST_F( M6502StoreRegisterTests, STAAbsoluteCanStoreTheARegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterAbsolute( CPU::INS_STA_ABS, &CPU::A );
}

TEST_F( M6502StoreRegisterTests, STXAbsoluteCanStoreTheXRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterAbsolute( CPU::INS_STX_ABS, &CPU::X );
}

TEST_F( M6502StoreRegisterTests, STYAbsoluteCanStoreTheYRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterAbsolute( CPU::INS_STY_ABS, &CPU::Y );
}

TEST_F( M6502StoreRegisterTests, STAZeroPageXCanStoreTheARegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPageX( CPU::INS_STA_ZPX, &CPU::A );
}

TEST_F( M6502StoreRegisterTests, STYZeroPageXCanStoreTheYRegisterIntoMemory )
{
	using namespace m6502;
	tau->TestStoreRegisterZeroPageX( CPU::INS_STY_ZPX, &CPU::Y );
}

TEST_F( M6502StoreRegisterTests, STAAbsoluteXCanStoreTheARegisterIntoMemory )
{
	// given:
	using namespace m6502;
	tau->cpu.A = 0x42;
	tau->cpu.X = 0x0F;
	tau->mem[0xFFFC] = CPU::INS_STA_ABSX;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[0x800F], 0x42 );
	VerfifyUnmodifiedFlagsFromStoreRegister( tau->cpu, CPUCopy );
}

TEST_F( M6502StoreRegisterTests, STAAbsoluteYCanStoreTheARegisterIntoMemory )
{
	// given:
	using namespace m6502;
	tau->cpu.A = 0x42;
	tau->cpu.Y = 0x0F;
	tau->mem[0xFFFC] = CPU::INS_STA_ABSY;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[0x800F], 0x42 );
	VerfifyUnmodifiedFlagsFromStoreRegister( tau->cpu, CPUCopy );
}

TEST_F( M6502StoreRegisterTests, STAIndirectXCanStoreTheARegisterIntoMemory )
{
	// given:
	using namespace m6502;
	tau->cpu.A = 0x42;
	tau->cpu.X = 0x0F;
	tau->mem[0xFFFC] = CPU::INS_STA_INDX;
	tau->mem[0xFFFD] = 0x20;
	tau->mem[0x002F] = 0x00;
	tau->mem[0x0030] = 0x80;
	tau->mem[0x8000] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[0x8000], 0x42 );
	VerfifyUnmodifiedFlagsFromStoreRegister( tau->cpu, CPUCopy );
}

TEST_F( M6502StoreRegisterTests, STAIndirectYCanStoreTheARegisterIntoMemory )
{
	// given:
	using namespace m6502;
	tau->cpu.A = 0x42;
	tau->cpu.Y = 0x0F;
	tau->mem[0xFFFC] = CPU::INS_STA_INDY;
	tau->mem[0xFFFD] = 0x20;
	tau->mem[0x0020] = 0x00;
	tau->mem[0x0021] = 0x80;
	tau->mem[0x8000 + 0x0F] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_EQ( tau->mem[0x8000 + 0x0F], 0x42 );
	VerfifyUnmodifiedFlagsFromStoreRegister( tau->cpu, CPUCopy );
}
