#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "alu/Valu.h"
#include "verilated.h"

#include "inc/testbench.hpp"
#define MODULE Valu

void logOperation(MODULE* module)
{
	printf("A_in=0x%02X ",module->A_in);
	printf("B_in=0x%02X ",module->B_in);
	printf("C_out=0x%02X\n",module->C_out);
}

int main(int argc, char **argv)
{
	Verilated::commandArgs(argc, argv); // call Verilated Commands

	MODULE* model = new MODULE; // Make a new instance of the module under test
	auto tb = TestBench<MODULE>(model); // Load the module into the test bench
	model->A_in=0x0A;
	model->B_in=0xF0;
	tb.tick();  //Update the testbench
	logOperation(model); // output the result of the addition

	return 0;
}