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

template<class T>
void latchDataLine(SyncTB<MODTYPE>* tb, T* dataLine){
    *dataLine=1;
    tb->tick();
    tb->tick();
    *dataLine=0;
}

void cyclePhase2(SyncTB<MODTYPE>* tb){
    tb->dut->phase_2_rising=1;
    tb->tick();
    tb->dut->phase_2_rising=0;
}

//Cases to Test
//Program counter reset
TEST_CASE("Reset Functional","[PC]"){
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a new module test bench
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0000); //Assert that the PC initializes to zero
    auto newPCValue=0xF030;
    tb->dut->ProgramCounter__DOT__program_counter=newPCValue; // assign a new address to the PC
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==newPCValue); //Make sure the address updated
    tb->reset(); // Reset the device
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0000); //Assert that the program counter address reset on a reset    
}

//Incrementing the PC when in phase 2 of the clock
TEST_CASE("PC Increments on Phase 2 rising edge","[PC]"){
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a new module test bench

    //Load the current PC address into the next address
    latchDataLine(tb, &tb->dut->pcl_pcl);
    latchDataLine(tb, &tb->dut->pch_pch);

    tb->dut->increment_pc=1; // Set the program counter to increment on the next phase 2;
    tb->tick();
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0000); // require that the PC address hasn't updated
    cyclePhase2(tb);
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0001); // on a phase 2 rising edge the address updates
    cyclePhase2(tb);
    //If we don't relatch the increment register nothing changes
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0001);

    tb->dut->increment_pc=0;
    cyclePhase2(tb);
    //If we drop the increment flag back to zero without updating the address it goes back to zero
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter==0x0000);
}

TEST_CASE("Latching the Program Counter Select Register","[PC]"){
    auto* tb = new SyncTB<MODTYPE>(50000000,false); // make a new module test bench
    tb->dut->ProgramCounter__DOT__program_counter=0xDEAD;
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter_select==0x0000); //register initializes to zero

    //latching PCS from PC
    //TODO: Figure out if the latch is working right, plot with GTKWAVE
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter_select==0xDE00);
    latchDataLine(tb,&tb->dut->pcl_pcl);
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter_select==0xDEAD);

    //latching PCS from the Address bus
    tb->reset(); //reset the system
    tb->dut->address_l=0xEF;
    tb->dut->address_h=0xBE;
    latchDataLine(tb, &tb->dut->adh_pch);
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter_select==0xBE00);
    latchDataLine(tb, &tb->dut->adl_pcl);
    REQUIRE(tb->dut->ProgramCounter__DOT__program_counter_select==0xBEEF);
}

//Keeping the program counter constant on phase 2 of the clock
//Loading the next PC Address from the existing PC Address
//Loading the next PC Address from the address bus
