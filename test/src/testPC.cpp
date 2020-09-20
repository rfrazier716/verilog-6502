#define MODTYPE VProgramCounter

// Standard IO Includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Necessary Includes for Verilated
#include "ProgramCounter/VProgramCounter.h"
#include "verilated.h"

// Test Bench Class Definition and Implementation
#define CLOCK_LINE sys_clock //Define what clock line the testbench will toggle
#include "inc/SynchronousTB.hpp"

// Catch Library
#include "inc/catch.hpp"

TEST_CASE("Rising Edge Functional","[aSyncLatch]"){
    //When a rising edge occurs, on the next clock cycle it will the rising edge bit will be set
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a testbench of the module
    tb->reset(); //reset the program counter
    tb->dut->pch_pch=1;
    tb->dut->pcl_pcl=1;
    tb->tick();
    tb->tick();
    tb->dut->increment_pc=1;
    tb->dut->phase_2_rising=1;
    tb->tick();
    printf("PC_ADDR=0x%04X\n",tb->dut->ProgramCounter__DOT__program_counter);
    tb->tick();
    printf("PC_ADDR=0x%04X\n",tb->dut->ProgramCounter__DOT__program_counter);
    tb->tick();
    printf("PC_ADDR=0x%04X\n",tb->dut->ProgramCounter__DOT__program_counter);
}