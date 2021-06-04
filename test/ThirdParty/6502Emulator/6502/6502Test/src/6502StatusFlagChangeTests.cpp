#include <tau/tau.h>
#include "m6502.h"

struct M6502StatusFlagChangeTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502StatusFlagChangeTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502StatusFlagChangeTests){}


TEST_F( M6502StatusFlagChangeTests, CLCWillClearTheCarryFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.C = true;
	tau->mem[0xFF00] = CPU::INS_CLC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( tau->cpu.Flag.C );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( tau->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SECWillSetTheCarryFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.C = false;
	tau->mem[0xFF00] = CPU::INS_SEC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( tau->cpu.Flag.C );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( tau->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLDWillClearTheDecimalFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.D = true;
	tau->mem[0xFF00] = CPU::INS_CLD;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( tau->cpu.Flag.D );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( tau->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SEDWillSetTheDecimalFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.D = false;
	tau->mem[0xFF00] = CPU::INS_SED;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( tau->cpu.Flag.D );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( tau->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLIWillClearTheInterruptFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.I = true;
	tau->mem[0xFF00] = CPU::INS_CLI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( tau->cpu.Flag.I );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( tau->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SEIWillSetTheInterruptFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.I = false;
	tau->mem[0xFF00] = CPU::INS_SEI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( tau->cpu.Flag.I );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( tau->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLVWillClearTheOverflowFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.Reset( 0xFF00, tau->mem );
	tau->cpu.Flag.V = true;
	tau->mem[0xFF00] = CPU::INS_CLV;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = tau->cpu;

	// when:
	const s32 ActualCycles = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( tau->cpu.Flag.V );
	CHECK_EQ( tau->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( tau->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( tau->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( tau->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( tau->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( tau->cpu.Flag.N, CPUCopy.Flag.N );
}

