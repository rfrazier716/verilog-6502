#define MODTYPE Valu

// Standard IO Includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Necessary Includes for Verilated
#include "alu/Valu.h"
#include "verilated.h"

// Test Bench Class Definition and Implementation
#include "inc/testbench.hpp"

// Catch Library
#include "inc/catch.hpp"

void logOperation(MODTYPE* module)
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
}

TEST_CASE("ALU Add Operation Functional","[alu]")
{
	auto* tb = new TestBench<MODTYPE>(); // make a testbench of our module
	SECTION("Add without a carry in is the sum of A&B")
	{
		zeroALUOps(tb->dut);
		tb->dut->ADD_en = 1;
		tb->dut->A_in = 0x03;
		tb->dut->B_in = 0x08; 
		tb->dut->Carry_in = 0x00; //No carry in
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x0B);
		REQUIRE(tb->dut->Carry_out == 0);
	}
	
	SECTION("Add with a carry in is the sum of A&B+1")
	{
		zeroALUOps(tb->dut);
		tb->dut->ADD_en = 1;
		tb->dut->A_in = 0x03;
		tb->dut->B_in = 0x08; 
		tb->dut->Carry_in = 1; //carry in bit
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x0C);
		REQUIRE(tb->dut->Carry_out == 0);
	}

	SECTION("Carry out is set when the result is greater than 8 bits")
	{
		zeroALUOps(tb->dut);
		tb->dut->ADD_en = 1;
		tb->dut->A_in = 0xFD;
		tb->dut->B_in = 0x08; 
		tb->dut->Carry_in = 0; //carry in bit
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x05);
		REQUIRE(tb->dut->Carry_out == 1);
	}

	SECTION("Overflow is set only when signed math results in the wrong sign")
	{
		zeroALUOps(tb->dut);
		tb->dut->ADD_en = 1;

		// 2x positive numbers result in a negative number
		tb->dut->A_in = 0x7F; 
		tb->dut->B_in = 0x04; 
		tb->dut->Carry_in = 0; //carry in bit
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x83);
		REQUIRE(tb->dut->Carry_out == 0);
		REQUIRE(tb->dut->OVFLW == 1);

		// 2x negative numbers result in a positive number
		tb->dut->A_in = 0xFF; 
		tb->dut->B_in = 0x80; 
		tb->dut->Carry_in = 0; //carry in bit
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x7F);
		REQUIRE(tb->dut->Carry_out == 1);
		REQUIRE(tb->dut->OVFLW == 1); 

		// Carry-out can be set independent of the overflow bit
		tb->dut->A_in = 0x81; 
		tb->dut->B_in = 0x7F; 
		tb->dut->Carry_in = 0; //carry in bit
		tb->tick(); //update model

		REQUIRE(tb->dut->C_out == 0x00);
		REQUIRE(tb->dut->Carry_out == 1);
		REQUIRE(tb->dut->OVFLW == 0); 

	}
}

TEST_CASE("ALU XOR Operation Functional","[alu]")
{
	auto* tb = new TestBench<MODTYPE>(); // make a testbench of the module

	// Initialize ALU state
	zeroALUOps(tb->dut);
	tb->dut->XOR_en = 1;
	tb->dut->Carry_in = 0;

	SECTION("XOR Agrees with C implemenation and does not touch output flags")
	{
		auto xorWorking = true; // predefine the XOR working to true
		for(uint16_t a=0;a<=0xFF;a++)
		{
			for(uint16_t b=0;b<=0xFF;b++)
			{
				tb->dut->A_in=a;
				tb->dut->B_in=b;
				tb->tick();
				if(tb->dut->C_out != (a^b) || tb->dut->Carry_out == 1)
				{
					printf("XOR Assertion Failed with given inputs (expected 0x%04X): ", a^b);
					logOperation(tb->dut);
				}
				
			}
		}
		REQUIRE(xorWorking); // Require testcase never set to false;
	}
}

TEST_CASE("ALU OR Operation Functional","[alu]"){
	auto* tb = new TestBench<MODTYPE>(); // make a testbench of the module

	// Initialize ALU state
	zeroALUOps(tb->dut);
	tb->dut->OR_en = 1;
	tb->dut->Carry_in = 0;

	SECTION("OR Agrees with C implemenation and does not touch output flags"){
		auto orWorking = true; // predefine the XOR working to true
		for(uint16_t a=0;a<=0xFF;a++)
		{
			for(uint16_t b=0;b<=0xFF;b++)
			{
				tb->dut->A_in=a;
				tb->dut->B_in=b;
				tb->tick();
				if(tb->dut->C_out != (a | b) || tb->dut->Carry_out == 1 || tb->dut->OVFLW == 1)
				{

					printf("OR Assertion Failed with given inputs (expected 0x%02X): ", a|b);
					logOperation(tb->dut);
				}
				
			}
		}
		REQUIRE(orWorking); // Require testcase never set to false;
	}
}

TEST_CASE("ALU AND Operation Functional","[alu]"){
	auto* tb = new TestBench<MODTYPE>(); // make a testbench of the module

	// Initialize ALU state
	zeroALUOps(tb->dut);
	tb->dut->AND_en = 1;
	tb->dut->Carry_in = 0;

	SECTION("AND Agrees with C implemenation and does not touch output flags"){
		auto andWorking = true; // predefine the XOR working to true
		for(uint16_t a=0;a<=0xFF;a++)
		{
			for(uint16_t b=0;b<=0xFF;b++)
			{
				tb->dut->A_in=a;
				tb->dut->B_in=b;
				tb->tick();
				if(tb->dut->C_out != (a & b) || tb->dut->Carry_out == 1 || tb->dut->OVFLW == 1)
				{
					andWorking=false;
					printf("AND Assertion Failed with given inputs (expected 0x%02X): ", a&b);
					logOperation(tb->dut);
				}
				
			}
		}
		REQUIRE(andWorking); // Require testcase never set to false;
	}
}

TEST_CASE("ALU Shift Right Operation Functional","[alu]"){
	auto* tb = new TestBench<MODTYPE>(); // make a testbench of the module

	// Initialize ALU state
	zeroALUOps(tb->dut);
	tb->dut->SR_en = 1;
	tb->dut->Carry_in = 0;

	SECTION("SRS Agrees with C implemenation and does not touch output flags"){
		auto srsWorking = true; // predefine the XOR working to true
		for(uint16_t a=0;a<=0xFF;a++)
		{
				tb->dut->A_in=a;
				tb->tick();
				auto expectedCarry=a&0x01;

				if(!((tb->dut->C_out == (a>>1)) && (tb->dut->Carry_out == expectedCarry)))
				{
					srsWorking = false;
					printf("SRS Assertion Failed with given inputs (expected 0x%02X with carry[%01d]): ", a>>1, expectedCarry);
					logOperation(tb->dut);
				}
		}
		REQUIRE(srsWorking); // Require testcase never set to false;
	}
}