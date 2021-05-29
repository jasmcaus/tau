#include <Muon/Muon.h>
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
	muon->cpu.Reset( muon->mem );
}

TEST_F_TEARDOWN(M6502AddSubWithCarryTests){}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddZeroToZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddCarryAndZeroToZeroAndGetOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 1;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE(-17);
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddOneToFFAndItWillCauseACarry )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0xFF;			
	Test.Operand = 1;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheNegativeFlagWhenTheResultIsNegative )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = BYTE(-1);
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionFails )
{
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 01111111 127
	// C:1 N:0 V:1 Z:0
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, 
	ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionPassedDueToInitalCarryFlag )
{
	// C: 00000001
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 10000000 -128
	// C:1 N:1 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = BYTE(-128);
	Test.ExpectC = true;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedPositiveAddtionFails )
{
	// A: 01111111 127   
	// O: 00000001 1
	// =: 10000000 128
	// C:0 N:1 V:1 Z:0
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 127;
	Test.Operand = 1;
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestADCIndirectY( Test );
}

// SBC Abs --------------

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-128);
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE(-1);
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-20);
	Test.Operand = BYTE(-17);
	Test.Answer = BYTE(-3);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

// SBC Zero Page

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPage( Test );
}

// SBC Immediate ---------

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCImmediate( Test );
}

// SBC Zero Page, X -------

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCZeroPageX( Test );
}

// SBC Absolute, X -------

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

// SBC Absolute, Y

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCAbsoluteY( Test );
}

// SBC Indirect, X ---------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectX( Test );
}

// SBC Indirect, Y -----------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndGetZero )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroAndGetMinusOne )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	muon->ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoUnsignedNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbers )
{
	muon->ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	muon->TestSBCIndirectY( Test );
}