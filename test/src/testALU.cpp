#define MODTYPE Valu

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "alu/Valu.h"
#include "verilated.h"

#include "inc/testbench.hpp"



void logOperation(MODTYPE* module)
{
	printf("A_in=0x%02X ",module->A_in);
	printf("B_in=0x%02X ",module->B_in);
	printf("C_out=0x%02X ",module->C_out);
	printf("flags=%1d%1d\n",module->OVFLW, module->Carry_out);
}

int main(int argc, char **argv)
{
	Verilated::commandArgs(argc, argv); // call Verilated Commands

	MODTYPE* model = new MODTYPE; // Make a new instance of the module under test
	auto tb = TestBench<MODTYPE>(model); // Load the module into the test bench
	for(int a=0;a<=0xFF;a+=0x01)
	{
		for(int b=0;b<=0xFF;b+=0x01)
		{
			model->A_in=a;
			model->B_in=b;
			tb.tick();  //Update the testbench
			//logOperation(model); // output the result of the addition
		}
	}

		//Checking for overflow flag + Carry
		model->A_in=0xFF;
		model->B_in=0x80;
		tb.tick();
		logOperation(model);
		//Checking for overflow flag w/o carry
		model->A_in=0x7F;
		model->B_in=0x40;
		tb.tick();
		logOperation(model);

	return 0;
}