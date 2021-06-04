#include <tau/tau.h>
#include "m6502.h"

struct M6502AddSubWithCarryTests {
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	void ExpectUnaffectedRegisters( m6502::CPU CPUBefore )
	{
		CHECK_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		CHECK_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		CHECK_EQ( CPUBefore.Flag.B, cpu.Flag.B );
	}

	struct ADCTestData
	{
		bool Carry;
		m6502::Byte A;
		m6502::Byte Operand;
		m6502::Byte Answer;

		bool ExpectC;
		bool ExpectZ;
		bool ExpectN;
		bool ExpectV;
	};

	enum struct EOperation
	{
		Add, Subtract
	};

	void TestSBCAbsolute( ADCTestData Test )
	{		
		TestADCAbsolute( Test, EOperation::Subtract );
	}

	void TestADCAbsolute( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] = 
			(Operation == EOperation::Add) ? 
			CPU::INS_ADC_ABS : CPU::INS_SBC_ABS;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCAbsoluteX( ADCTestData Test )
	{
		TestADCAbsoluteX( Test, EOperation::Subtract );
	}

	void TestADCAbsoluteX( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ABSX : CPU::INS_SBC_ABSX;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000+0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCAbsoluteY( ADCTestData Test )
	{
		TestADCAbsoluteY( Test, EOperation::Subtract );
	}

	void TestADCAbsoluteY( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.Y = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ABSY : CPU::INS_SBC_ABSY;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000 + 0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCImmediate( ADCTestData Test )
	{
		TestADCImmediate( Test, EOperation::Subtract );
	}

	void TestADCImmediate( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC : CPU::INS_SBC;
		mem[0xFF01] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 2;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCZeroPage( ADCTestData Test )
	{
		TestADCZeroPage( Test, EOperation::Subtract );
	}

	void TestADCZeroPage( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] = 
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ZP : CPU::INS_SBC_ZP;
		mem[0xFF01] = 0x42;
		mem[0x0042] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCZeroPageX( ADCTestData Test )
	{
		TestADCZeroPageX( Test, EOperation::Subtract );
	}

	void TestADCZeroPageX( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ZPX : CPU::INS_SBC_ZPX;
		mem[0xFF01] = 0x42;
		mem[0x0042+0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCIndirectX( ADCTestData Test )
	{
		TestADCIndirectX( Test, EOperation::Subtract );
	}

	void TestADCIndirectX( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x04;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_INDX : CPU::INS_SBC_INDX;
		mem[0xFF01] = 0x02;
		mem[0x0006] = 0x00;	//0x2 + 0x4
		mem[0x0007] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCIndirectY( ADCTestData Test )
	{
		TestADCIndirectY( Test, EOperation::Subtract );
	}

	void TestADCIndirectY( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.Y = 0x04;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_INDY : CPU::INS_SBC_INDY;
		mem[0xFF01] = 0x02;
		mem[0x0002] = 0x00;
		mem[0x0003] = 0x80;
		mem[0x8000 + 0x04] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		CHECK_EQ( ActualCycles, EXPECTED_CYCLES );
		CHECK_EQ( cpu.A, Test.Answer );
		CHECK_EQ( cpu.Flag.C, Test.ExpectC );
		CHECK_EQ( cpu.Flag.Z, Test.ExpectZ );
		CHECK_EQ( cpu.Flag.N, Test.ExpectN );
		CHECK_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}
};

#define BYTE( A ) ( (m6502::Byte)A )

TEST_F_SETUP(M6502AddSubWithCarryTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502AddSubWithCarryTests){}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddZeroToZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddCarryAndZeroToZeroAndGetOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 1;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE(-17);
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddOneToFFAndItWillCauseACarry )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0xFF;			
	Test.Operand = 1;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheNegativeFlagWhenTheResultIsNegative )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = BYTE(-1);
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionFails )
{
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 01111111 127
	// C:1 N:0 V:1 Z:0
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, 
	ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionPassedDueToInitalCarryFlag )
{
	// C: 00000001
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 10000000 -128
	// C:1 N:1 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = BYTE(-128);
	Test.ExpectC = true;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedPositiveAddtionFails )
{
	// A: 01111111 127   
	// O: 00000001 1
	// =: 10000000 128
	// C:0 N:1 V:1 Z:0
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 127;
	Test.Operand = 1;
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestADCIndirectY( Test );
}

// SBC Abs --------------

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-128);
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE(-1);
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-20);
	Test.Operand = BYTE(-17);
	Test.Answer = BYTE(-3);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsolute( Test );
}

// SBC Zero Page

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPage( Test );
}

// SBC Immediate ---------

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCImmediate( Test );
}

// SBC Zero Page, X -------

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCZeroPageX( Test );
}

// SBC Absolute, X -------

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteX( Test );
}

// SBC Absolute, Y

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCAbsoluteY( Test );
}

// SBC Indirect, X ---------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectX( Test );
}

// SBC Indirect, Y -----------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndGetZero )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroAndGetMinusOne )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoUnsignedNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbers )
{
	M6502AddSubWithCarryTests::ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	tau->TestSBCIndirectY( Test );
}