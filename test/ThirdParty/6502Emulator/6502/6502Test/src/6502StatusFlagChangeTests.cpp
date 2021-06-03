#include <tau/tau.h>
#include "m6502.h"

struct M6502StatusFlagChangeTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502StatusFlagChangeTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502StatusFlagChangeTests){}


TEST_F( M6502StatusFlagChangeTests, CLCWillClearTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = true;
	muon->mem[0xFF00] = CPU::INS_CLC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.C );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( muon->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SECWillSetTheCarryFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.C = false;
	muon->mem[0xFF00] = CPU::INS_SEC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( muon->cpu.Flag.C );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( muon->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLDWillClearTheDecimalFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.D = true;
	muon->mem[0xFF00] = CPU::INS_CLD;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.D );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( muon->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SEDWillSetTheDecimalFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.D = false;
	muon->mem[0xFF00] = CPU::INS_SED;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( muon->cpu.Flag.D );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( muon->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLIWillClearTheInterruptFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.I = true;
	muon->mem[0xFF00] = CPU::INS_CLI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.I );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( muon->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, SEIWillSetTheInterruptFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.I = false;
	muon->mem[0xFF00] = CPU::INS_SEI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_TRUE( muon->cpu.Flag.I );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( muon->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.V, CPUCopy.Flag.V );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

TEST_F( M6502StatusFlagChangeTests, CLVWillClearTheOverflowFlag )
{
	// given:
	using namespace m6502;
	muon->cpu.Reset( 0xFF00, muon->mem );
	muon->cpu.Flag.V = true;
	muon->mem[0xFF00] = CPU::INS_CLV;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = muon->cpu;

	// when:
	const s32 ActualCycles = muon->cpu.Execute( EXPECTED_CYCLES, muon->mem );

	// then:
	CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
	CHECK_FALSE( muon->cpu.Flag.V );
	CHECK_EQ( muon->cpu.Flag.Z, CPUCopy.Flag.Z );
	CHECK_EQ( muon->cpu.Flag.D, CPUCopy.Flag.D );
	CHECK_EQ( muon->cpu.Flag.C, CPUCopy.Flag.C );
	CHECK_EQ( muon->cpu.Flag.B, CPUCopy.Flag.B );
	CHECK_EQ( muon->cpu.Flag.I, CPUCopy.Flag.I );
	CHECK_EQ( muon->cpu.Flag.N, CPUCopy.Flag.N );
}

