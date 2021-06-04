#include <tau/tau.h>
#include "m6502.h"

/** 
; TestPrg

* = $1000

lda #$FF

start
sta $90
sta $8000
eor #$CC
jmp start

*/
static m6502::Byte TestPrg[] = {
	0x00, 0x10, 0xA9, 0xFF, 0x85, 0x90,
	0x8D, 0x00, 0x80, 0x49, 0xCC, 0x4C, 0x02, 0x10 };

static const m6502::u32 NumBytesInPrg = 14;

struct M6502LoadPrgTests
{
public:	
	m6502::Mem mem;
	m6502::CPU cpu;
};

TEST_F_SETUP(M6502LoadPrgTests) {
	tau->cpu.Reset( tau->mem );
}

TEST_F_TEARDOWN(M6502LoadPrgTests){}

TEST_F( M6502LoadPrgTests, TestLoadProgramAProgramIntoTheCorrectAreaOfMemory )
{
	// given:
	using namespace m6502;

	// when:
	tau->cpu.LoadPrg( TestPrg, NumBytesInPrg, tau->mem );

	//then:
	CHECK_EQ( tau->mem[0x0FFF], 0x0 );
	CHECK_EQ( tau->mem[0x1000], 0xA9 );
	CHECK_EQ( tau->mem[0x1001], 0xFF );
	CHECK_EQ( tau->mem[0x1002], 0x85 );
	//....
	CHECK_EQ( tau->mem[0x1009], 0x4C );
	CHECK_EQ( tau->mem[0x100A], 0x02 );
	CHECK_EQ( tau->mem[0x100B], 0x10 );
	CHECK_EQ( tau->mem[0x100C], 0x0 );
}

TEST_F( M6502LoadPrgTests, TestLoadProgramAProgramAndExecuteIt )
{
	// given:
	using namespace m6502;

	// when:
	Word StartAddress = tau->cpu.LoadPrg( TestPrg, NumBytesInPrg, tau->mem );
	tau->cpu.PC = StartAddress;

	//then:
	for ( m6502::s32 Clock = 1000; Clock > 0; )
	{
		Clock -= tau->cpu.Execute( 1, tau->mem );
	}
}

TEST_F( M6502LoadPrgTests, LoadThe6502TestPrg )
{
#if 0
	// given:
	using namespace m6502;

	// when:
	FILE* fp;
	fopen_s( &fp, 
		"6502_functional_test.bin", "rb" );

	fread( &tau->mem[0x000A], 1, 65526, fp );
	fclose( fp );

	tau->cpu.PC = 0x400;

	//then:
	while ( true )
	{
		tau->cpu.Execute( 1, tau->mem );
	}
#endif
}