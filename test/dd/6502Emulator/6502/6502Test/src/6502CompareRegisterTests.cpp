#include <Muon/Muon.h>
#include "m6502.h"

struct M6502CompareRegisterTests
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	void ExpectUnaffectedRegisters( m6502::CPU CPUBefore )
	{
		CHECK_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		CHECK_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		CHECK_EQ( CPUBefore.Flag.B, cpu.Flag.B );
		CHECK_EQ( CPUBefore.Flag.V, cpu.Flag.V );
	}

	struct CMPTestData
	{
		m6502::Byte RegisterValue;
		m6502::Byte Operand;

		bool ExpectC;
		bool ExpectZ;
		bool ExpectN;
	};

	CMPTestData CompareTwoIdenticalValues()
	{
		CMPTestData Test;
		Test.RegisterValue = 26;
		Test.Operand = 26;
		Test.ExpectZ = true;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareALargePositiveToASmallPositive()
	{
		CMPTestData Test;
		Test.RegisterValue = 48;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareANegativeNumberToAPositive()
	{
		CMPTestData Test;
		Test.RegisterValue = 130; //Negative number!
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareTwoValuesThatResultInANegativeFlagSet()
	{
		CMPTestData Test;
		Test.RegisterValue = 8;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = true;
		Test.ExpectC = false;
		return Test;
	}

	enum struct ERegister
	{
		A, X, Y
	};

	void CompareImmediate( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY;
			break;
		};
		*Register = Test.RegisterValue;

		mem[0xFF00] = Opcode;
		mem[0xFF01] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 2;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( *Register, Test.RegisterValue );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CompareZeroPage( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		
		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP_ZP;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX_ZP;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY_ZP;
			break;
		};
		*Register = Test.RegisterValue;
		mem[0xFF00] = Opcode;
		mem[0xFF01] = 0x42;
		mem[0x0042] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( *Register, Test.RegisterValue );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPZeroPageX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_ZPX;
		mem[0xFF01] = 0x42;
		mem[0x0042+0x4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.RegisterValue );
		CHECK_EQ( cpu.X, 4 );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CompareAbsolute( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;

		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP_ABS;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX_ABS;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY_ABS;
			break;
		};
		*Register = Test.RegisterValue;

		mem[0xFF00] = Opcode;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( *Register, Test.RegisterValue );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPAbsoluteX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_ABSX;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000+4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.RegisterValue );
		CHECK_EQ( cpu.X, 4 );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPAbsoluteY( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.Y = 4;
		mem[0xFF00] = CPU::INS_CMP_ABSY;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000 + 4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.RegisterValue );
		CHECK_EQ( cpu.Y, 4 );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPIndirectX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_INDX;
		mem[0xFF01] = 0x42;
		mem[0x42 + 4] = 0x00;
		mem[0x42 + 5] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.RegisterValue );
		CHECK_EQ( cpu.X, 4 );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPIndirectY( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.Y = 4;
		mem[0xFF00] = CPU::INS_CMP_INDY;
		mem[0xFF01] = 0x42;
		mem[0x42] = 0x00;
		mem[0x43] = 0x80;
		mem[0x8000+4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.RegisterValue );
		CHECK_EQ( cpu.Y, 4 );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}
};

//-- Immediate

TEST_F_SETUP(M6502CompareRegisterTests) {
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502CompareRegisterTests){}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoIdenticalValues  )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::A );
}

//-- Zero Page

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::A );
}

//-- Zero Page X

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CMPZeroPageX( Test );
}

//-- Absolute

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::A );
}

//-- Absolute X

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CMPAbsoluteX( Test );
}

//-- Absolute Y

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CMPAbsoluteY( Test );
}

//-- Indirect X

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CMPIndirectX( Test );
}

//-- Indirect Y

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CMPIndirectY( Test );
}

//-- CPX Immediate

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::X );
}

//-- CPY Immediate

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareImmediate( Test, M6502CompareRegisterTests::ERegister::Y );
}

//-- CPX Zero Page

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::X );
}

//-- CPY Zero Page

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareZeroPage( Test, M6502CompareRegisterTests::ERegister::Y );
}

//-- CPX Absolute

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::X );
}

//-- CPY Absolute

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareTwoIdenticalValues )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoIdenticalValues();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareALargePositiveToASmallPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareALargePositiveToASmallPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareANegativeNumberToAPositive )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareANegativeNumberToAPositive();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	M6502CompareRegisterTests::CMPTestData Test = muon->CompareTwoValuesThatResultInANegativeFlagSet();
	muon->CompareAbsolute( Test, M6502CompareRegisterTests::ERegister::Y );
}

#if 0 // for loop test
TEST_F( M6502CompareRegisterTests, LoopTest )
{
	// given:
	using namespace m6502;
	/* 
	* = $1000

	lda #0
	clc
	loop
	   adc #8
	   cmp #24
	   bne loop

	ldx #20
	*/
	Byte TestPrg[] = { 0x0,0x10,0xA9,0x00,0x18,0x69,0x08,0xC9,0x18,0xD0,0xFA,0xA2,0x14 };

	// when:
	Word StartAddress = muon->cpu.LoadPrg( TestPrg, sizeof( TestPrg ), muon->mem );
	muon->cpu.PC = StartAddress;

	//then:
	for ( m6502::s32 Clock = 1000; Clock > 0; )
	{
		Clock -= muon->cpu.Execute( 1, muon->mem );
	}
}
#endif