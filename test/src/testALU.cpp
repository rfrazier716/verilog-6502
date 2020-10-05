#define MODTYPE Valu

// Standard IO Includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Necessary Includes for Verilated
#include "alu/Valu.h"
#include "verilated.h"

// Test Bench Class Definition and Implementation
#define CLOCK_LINE i_clk
#include "inc/WishBoneTB.hpp"

// Catch Library
#include "inc/catch.hpp"

/*void logOperation(MODTYPE* module)
{
	printf("A_in=0x%02X ",module->A_in);
	printf("B_in=0x%02X ",module->B_in);
	printf("C_out=0x%02X ",module->C_out);
	printf("flags=%1d%1d\n",module->OVFLW, module->Carry_out);
}

void zeroALUOps(MODTYPE* dut)
{
	dut->ADD_en=0;
	dut->AND_en=0;
	dut->XOR_en=0;
	dut->OR_en=0;
	dut->SR_en=0;
}*/

TEST_CASE("ALU Wishbone Write Functional","[alu]")
{
	auto* tb = new WishboneTB<MODTYPE>(50000000); // make a testbench of our module
	tb->addVCDTrace("WishboneWrite.vcd");
	tb->tick();
	tb->wbSingleWrite(0x00, 0xA0);
	tb->wbSingleWrite(0x01, 0xF0);
	tb->wbSingleRead(0x80); // Read an add with no carry
	tb->tick();
	tb->wbSingleWrite(0x00, 0x80);
	tb->wbSingleWrite(0x01, 0x81);
	tb->wbSingleRead(0x80); // Read an add with no carry
	tb->tick();
	tb->wbSingleRead(0x02); // Read the flags register
}