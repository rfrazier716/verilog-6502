#define MODTYPE VAsynchronousLatch

// Standard IO Includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Necessary Includes for Verilated
#include "AsynchronousLatch/VAsynchronousLatch.h"
#include "verilated.h"

// Test Bench Class Definition and Implementation
#define CLOCK_LINE sys_clock //Define what clock line the testbench will toggle
#include "inc/SynchronousTB.hpp"

// Catch Library
#include "inc/catch.hpp"

void logOperation(MODTYPE* module)
{
    printf("input signal=0x%02X ",module->async_signal);
	printf("rising edge=0x%02X ",module->rising_edge);
	printf("falling edge=0x%02X\n",module->falling_edge);
}

TEST_CASE("Rising Edge Functional","[aSyncLatch]"){
    //When a rising edge occurs, on the next clock cycle it will the rising edge bit will be set
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a testbench of the module
    tb->addVCDTrace("RisingEdgeLatch.vcd");
    tb->tick(); //initial clock line
    tb->dut->async_signal=1; //Drive the asynchronous Signal high
    tb->tick();
    REQUIRE(tb->dut->rising_edge == 0x00); //rising edge is not immediately set
    tb->tick();
    REQUIRE(tb->dut->rising_edge == 0x01);

    //The flag should only last for one clock cycle
    tb->tick();
    REQUIRE(tb->dut->rising_edge == 0x00);
}

TEST_CASE("Falling Edge Functional","[aSyncLatch]"){
    //When a falling edge occurs, on the next clock cycle it will the rising edge bit will be set
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a testbench of the module
    tb->dut->async_signal=1; //Drive the asynchronous Signal high
    tb->tick();
    tb->dut->async_signal=0; //Drive the asynchronous Signal low
    tb->tick();
    tb->tick();
    REQUIRE(tb->dut->falling_edge == 0x01);

    //The flag should only last for one clock cycle
    tb->tick();
    REQUIRE(tb->dut->rising_edge == 0x00);
}

TEST_CASE("Reset Functional","[aSyncLatch]"){
    //When a reset occurs the shift register is reinitialized to 3'b000
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a testbench of the module
    tb->dut->async_signal=1; //Drive the asynchronous Signal high
    for(int i=0;i<3;i++){
        //Execute multiple clock ticks to fill the shift register with logic 1's
        tb->tick();
    }
    REQUIRE(tb->dut->AsynchronousLatch__DOT__shift_register==0b111);
    tb->reset(); //Reset the system
    REQUIRE(tb->dut->AsynchronousLatch__DOT__shift_register==0b000); 
}
