#include <tau/tau.h>
#include "m6502.h"

struct M6502AndEorOraBitTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;

	static void VerfifyUnmodifiedFlagsFromLogicalOpInstruction(
		const m6502::CPU& cpu,
		const m6502::CPU& CPUCopy )
	{
		CHECK_EQ( cpu.Flag.C, CPUCopy.Flag.C );
		CHECK_EQ( cpu.Flag.I, CPUCopy.Flag.I );
		CHECK_EQ( cpu.Flag.D, CPUCopy.Flag.D );
		CHECK_EQ( cpu.Flag.B, CPUCopy.Flag.B );
		CHECK_EQ( cpu.Flag.V, CPUCopy.Flag.V );
	}

	enum struct ELogicalOp
	{
		And, Eor, Or
	};

	m6502::Byte DoLogicalOp( 
		m6502::Byte A, 
		m6502::Byte B, 
		ELogicalOp LogicalOp )
	{
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			return A & B;
			break;
		case ELogicalOp::Or:
			return A | B;
			break;
		case ELogicalOp::Eor:
			return A ^ B;
			break;
		}

		throw - 1; //invalid Logical Op
	}

	void TestLogicalOpImmediate( 
		ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_IM;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_IM;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_IM;
			break;
		}
		mem[0xFFFD] = 0x84;

		//when:
		CPU CPUCopy = cpu;
		s32 CyclesUsed = cpu.Execute( 2, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x84, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, 2 );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpZeroPage( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ZP;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ZP;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ZP;
			break;
		}
		mem[0xFFFD] = 0x42;
		mem[0x0042] = 0x37;

		//when:
		CPU CPUCopy = cpu;
		s32 CyclesUsed = cpu.Execute( 3, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, 3 );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpZeroPageX( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		cpu.X = 5;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ZPX;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ZPX;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ZPX;
			break;
		}
		mem[0xFFFD] = 0x42;
		mem[0x0047] = 0x37;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( 4, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, 4 );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpAbsolute( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.Flag.Z = cpu.Flag.N = true;
		cpu.A = 0xCC;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ABS;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ABS;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ABS;
			break;
		}
		mem[0xFFFD] = 0x80;
		mem[0xFFFE] = 0x44;	//0x4480
		mem[0x4480] = 0x37;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpAbsoluteX( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.Flag.Z = cpu.Flag.N = true;
		cpu.A = 0xCC;
		cpu.X = 1;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ABSX;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ABSX;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ABSX;
			break;
		}
		mem[0xFFFD] = 0x80;
		mem[0xFFFE] = 0x44;	//0x4480
		mem[0x4481] = 0x37;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpAbsoluteY( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.Flag.Z = cpu.Flag.N = true;
		cpu.A = 0xCC;
		cpu.Y = 1;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ABSY;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ABSY;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ABSY;
			break;
		}
		mem[0xFFFD] = 0x80;
		mem[0xFFFE] = 0x44;	//0x4480
		mem[0x4481] = 0x37;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLoadRegisterAbsoluteYWhenCrossingPage( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		cpu.Y = 0xFF;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ABSY;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ABSY;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ABSY;
			break;
		}
		mem[0xFFFD] = 0x02;
		mem[0xFFFE] = 0x44;	//0x4402
		mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLoadRegisterAbsoluteXWhenCrossingPage( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		cpu.X = 0xFF;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ABSX;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ABSX;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ABSX;
			break;
		}
		mem[0xFFFD] = 0x02;
		mem[0xFFFE] = 0x44;	//0x4402
		mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpIndirectX( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.Flag.Z = cpu.Flag.N = true;
		cpu.A = 0xCC;
		cpu.X = 0x04;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_INDX;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_INDX;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_INDX;
			break;
		}
		mem[0xFFFD] = 0x02;
		mem[0x0006] = 0x00;	//0x2 + 0x4
		mem[0x0007] = 0x80;
		mem[0x8000] = 0x37;
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpIndirectY( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.Flag.Z = cpu.Flag.N = true;
		cpu.A = 0xCC;
		cpu.Y = 0x04;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_INDY;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_INDY;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_INDY;
			break;
		}
		mem[0xFFFD] = 0x02;
		mem[0x0002] = 0x00;
		mem[0x0003] = 0x80;
		mem[0x8004] = 0x37;	//0x8000 + 0x4
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpIndirectYWhenItCrossesAPage( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		cpu.Y = 0xFF;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_INDY;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_INDY;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_INDY;
			break;
		}
		mem[0xFFFD] = 0x02;
		mem[0x0002] = 0x02;
		mem[0x0003] = 0x80;
		mem[0x8101] = 0x37;	//0x8002 + 0xFF
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		//when:
		s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}

	void TestLogicalOpZeroPageXWhenItWraps
		( ELogicalOp LogicalOp )
	{
		// given:
		using namespace m6502;
		cpu.A = 0xCC;
		cpu.X = 0xFF;
		switch ( LogicalOp )
		{
		case ELogicalOp::And:
			mem[0xFFFC] = CPU::INS_AND_ZPX;
			break;
		case ELogicalOp::Or:
			mem[0xFFFC] = CPU::INS_ORA_ZPX;
			break;
		case ELogicalOp::Eor:
			mem[0xFFFC] = CPU::INS_EOR_ZPX;
			break;
		}
		mem[0xFFFD] = 0x80;
		mem[0x007F] = 0x37;

		//when:
		CPU CPUCopy = cpu;
		s32 CyclesUsed = cpu.Execute( 4, mem );

		//then:
		const Byte ExpectedResult = DoLogicalOp( 0xCC, 0x37, LogicalOp );
		const bool ExpectedNegative = (ExpectedResult & 0b10000000) > 0;
		CHECK_EQ( cpu.A, ExpectedResult );
		CHECK_EQ( CyclesUsed, 4 );
		CHECK_FALSE( cpu.Flag.Z );
		CHECK_EQ( cpu.Flag.N, ExpectedNegative );
		VerfifyUnmodifiedFlagsFromLogicalOpInstruction( cpu, CPUCopy );
	}
};


TEST_F_SETUP(M6502AndEorOraBitTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502AndEorOraBitTests){}


TEST_F( M6502AndEorOraBitTests, TestLogicalOpANDOnARegisterImmediate )
{
	using namespace m6502;
	tau->TestLogicalOpImmediate( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOROnARegisterImmediate )
{
	using namespace m6502;
	tau->TestLogicalOpImmediate( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEOROnARegisterImmediate )
{
	using namespace m6502;
	tau->TestLogicalOpImmediate( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndOnARegisterZeroPage )
{
	using namespace m6502;
	tau->TestLogicalOpZeroPage( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrOnARegisterZeroPage )
{
	using namespace m6502;
	tau->TestLogicalOpZeroPage( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorOnARegisterZeroPage )
{
	using namespace m6502;
	tau->TestLogicalOpZeroPage( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorImmediateCanAffectZeroFlag )
{
	// given:
	using namespace m6502;
	tau->cpu.A = 0xCC;
	tau->mem[0xFFFC] = CPU::INS_EOR_IM;
	tau->mem[0xFFFD] = tau->cpu.A;
	CPU CPUCopy = tau->cpu;

	//when:
	tau->cpu.Execute( 2, tau->mem );

	//then:
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.N );
	tau->VerfifyUnmodifiedFlagsFromLogicalOpInstruction( tau->cpu, CPUCopy );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndOnARegisterZeroPageX )
{
	tau->TestLogicalOpZeroPageX( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrOnARegisterZeroPageX )
{
	tau->TestLogicalOpZeroPageX( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorOnARegisterZeroPageX )
{
	tau->TestLogicalOpZeroPageX( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, LogicalOpEorCanLoadAValueIntoTheARegisterWhenItWrapsZeroPageX )
{
	tau->TestLogicalOpZeroPageXWhenItWraps( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, LogicalOpOrCanLoadAValueIntoTheARegisterWhenItWrapsZeroPageX )
{
	tau->TestLogicalOpZeroPageXWhenItWraps( M6502AndEorOraBitTests::ELogicalOp::Or );
}
TEST_F( M6502AndEorOraBitTests, LogicalOpAndCanLoadAValueIntoTheARegisterWhenItWrapsZeroPageX )
{
	tau->TestLogicalOpZeroPageXWhenItWraps( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorOnARegisterAbsolute )
{
	tau->TestLogicalOpAbsolute( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrOnARegisterAbsolute )
{
	tau->TestLogicalOpAbsolute( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndOnARegisterAbsolute )
{
	tau->TestLogicalOpAbsolute( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndOnARegisterAbsoluteX )
{
	tau->TestLogicalOpAbsoluteX( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrOnARegisterAbsoluteX )
{
	tau->TestLogicalOpAbsoluteX( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorOnARegisterAbsoluteX )
{
	tau->TestLogicalOpAbsoluteX( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndWhenItCrossesAPageBoundaryAbsoluteX )
{
	tau->TestLoadRegisterAbsoluteXWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrWhenItCrossesAPageBoundaryAbsoluteX )
{
	tau->TestLoadRegisterAbsoluteXWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorWhenItCrossesAPageBoundaryAbsoluteX )
{
	tau->TestLoadRegisterAbsoluteXWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndAbsoluteY )
{
	tau->TestLogicalOpAbsoluteY( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrAbsoluteY )
{
	tau->TestLogicalOpAbsoluteY( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorAbsoluteY )
{
	tau->TestLogicalOpAbsoluteY( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndWhenItCrossesAPageBoundaryAbsoluteY )
{
	tau->TestLoadRegisterAbsoluteYWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrWhenItCrossesAPageBoundaryAbsoluteY )
{
	tau->TestLoadRegisterAbsoluteYWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorWhenItCrossesAPageBoundaryAbsoluteY )
{
	tau->TestLoadRegisterAbsoluteYWhenCrossingPage( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndIndirectX )
{
	tau->TestLogicalOpIndirectX( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorIndirectX )
{
	tau->TestLogicalOpIndirectX( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrIndirectX )
{
	tau->TestLogicalOpIndirectX( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndIndirectY )
{
	tau->TestLogicalOpIndirectY( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrIndirectY )
{
	tau->TestLogicalOpIndirectY( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorIndirectY )
{
	tau->TestLogicalOpIndirectY( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpAndWhenItCrossesAPageIndirectY )
{
	tau->TestLogicalOpIndirectYWhenItCrossesAPage( M6502AndEorOraBitTests::ELogicalOp::And );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpOrWhenItCrossesAPageIndirectY )
{
	tau->TestLogicalOpIndirectYWhenItCrossesAPage( M6502AndEorOraBitTests::ELogicalOp::Or );
}

TEST_F( M6502AndEorOraBitTests, TestLogicalOpEorWhenItCrossesAPageIndirectY )
{
	tau->TestLogicalOpIndirectYWhenItCrossesAPage( M6502AndEorOraBitTests::ELogicalOp::Eor );
}

TEST_F( M6502AndEorOraBitTests, TestBitZeroPage )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = false;
	tau->cpu.A = 0xCC;
	tau->mem[0xFFFC] = CPU::INS_BIT_ZP;	
	tau->mem[0xFFFD] = 0x42;
	tau->mem[0x0042] = 0xCC;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0xCC );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.V );
	CHECK_TRUE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitZeroPageResultZero )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = true;
	tau->cpu.A = 0xCC;
	tau->mem[0xFFFC] = CPU::INS_BIT_ZP;
	tau->mem[0xFFFD] = 0x42;
	tau->mem[0x0042] = 0x33;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0xCC );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.V );
	CHECK_FALSE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitZeroPageResultZeroBits6And7Zero )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = false;
	tau->cpu.A = 0x33;
	tau->mem[0xFFFC] = CPU::INS_BIT_ZP;
	tau->mem[0xFFFD] = 0x42;
	tau->mem[0x0042] = 0xCC;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x33 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.V );
	CHECK_TRUE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitZeroPageResultZeroBits6And7Mixed )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = false;
	tau->cpu.Flag.N = true;
	tau->mem[0xFFFC] = CPU::INS_BIT_ZP;
	tau->mem[0xFFFD] = 0x42;
	tau->mem[0x0042] = 0b01000000;
	constexpr s32 EXPECTED_CYCLES = 3;

	//when:
	tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_TRUE( tau->cpu.Flag.V );
	CHECK_FALSE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitAbsolute )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = false;
	tau->cpu.A = 0xCC;
	tau->mem[0xFFFC] = CPU::INS_BIT_ABS;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	tau->mem[0x8000] = 0xCC;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0xCC );
	CHECK_FALSE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.V );
	CHECK_TRUE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitAbsoluteResultZero )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = true;
	tau->cpu.A = 0xCC;
	tau->mem[0xFFFC] = CPU::INS_BIT_ABS;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	tau->mem[0x8000] = 0x33;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0xCC );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_FALSE( tau->cpu.Flag.V );
	CHECK_FALSE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitAbsoluteResultZeroBit6And7Zero )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = tau->cpu.Flag.N = false;
	tau->cpu.A = 0x33;
	tau->mem[0xFFFC] = CPU::INS_BIT_ABS;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	tau->mem[0x8000] = 0xCC;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	//when:
	s32 CyclesUsed = tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_EQ( CyclesUsed, EXPECTED_CYCLES );
	CHECK_EQ( tau->cpu.A, 0x33 );
	CHECK_TRUE( tau->cpu.Flag.Z );
	CHECK_TRUE( tau->cpu.Flag.V );
	CHECK_TRUE( tau->cpu.Flag.N );
}

TEST_F( M6502AndEorOraBitTests, TestBitAbsoluteResultZeroBit6And7Mixed )
{
	// given:
	using namespace m6502;
	tau->cpu.Flag.V = true;
	tau->cpu.Flag.N = false;
	tau->mem[0xFFFC] = CPU::INS_BIT_ABS;
	tau->mem[0xFFFD] = 0x00;
	tau->mem[0xFFFE] = 0x80;
	tau->mem[0x8000] = 0b10000000;
	CPU CPUCopy = tau->cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	//when:
	tau->cpu.Execute( EXPECTED_CYCLES, tau->mem );

	//then:
	CHECK_FALSE( tau->cpu.Flag.V );
	CHECK_TRUE( tau->cpu.Flag.N );
}