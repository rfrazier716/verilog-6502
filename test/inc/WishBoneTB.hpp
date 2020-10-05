#pragma once
#include "inc/SynchronousTB.hpp"


// The pipelined wishbone testbench inherits the synchronous test-bench with the additional wbWrite and wbRead methods
template <class T>
class WishboneTB: public SyncTB<T>
{
public:
    WishboneTB(int clockFrequency); // Class Constructor
    void wbSingleWrite(int wbAddress, int wbData);
    int wbSingleRead(int addr);
};

template<class T>
WishboneTB<T>::WishboneTB(int clockFrequency): SyncTB<T>(clockFrequency){
    this->dut = new T; // Assign the model the testbench manipulates
    this->tickCount=0; // set the tick count to zero
    Verilated::traceEverOn(true);
    this->tbTrace = NULL; //Null out the pointer
}

template<class T>
void WishboneTB<T>::wbSingleWrite(int wbAddress, int wbData)
{
    this->dut->i_wb_data = wbData; // put data onto bus
    this->dut->i_wb_addr = wbAddress; // put the address onto the address bus
    this->dut->i_wb_we = 1; // put write enable high 
    this->dut->i_wb_stb = 1; 
    this->tick(); // tick the clock
    this->dut->i_wb_we = 0; // assert write enable low again
    this->dut->i_wb_stb = 0; //lower the standby because the single cycle is over
    //this->tick();
}

template<class T>
int WishboneTB<T>::wbSingleRead(int wbAddress)
{
    int returnValue = 0x00;
    this->dut->i_wb_addr = wbAddress; // put the address onto the address bus
    this->dut->i_wb_we = 0; // put write enable low 
    this->dut->i_wb_stb = 1; 
    this->tick(); // tick the clock
    returnValue = this->dut->o_wb_data;
    this->dut->i_wb_stb = 0; //lower the standby because the single cycle is over
    //this->tick();
    return returnValue;
}