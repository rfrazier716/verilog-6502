#pragma once

//If the clock line wasn't already defined in the parent file define it here
#ifndef CLOCK_LINE
    #define CLOCK_LINE sys_clock
#endif

template<class T>
class SyncTB 
{
    bool traceOutput; //Whether or not to trace the output to a gtkwave file
    const int clockFrequency; //The system Clock frequency;
    double timeStep; //How long a timestep is, in nanoseconds

public:
    T* dut; // The device being tested
    void tick(); //Execute a clock cycle

    SyncTB(int clockFrequency, bool trace); //Class Constructor
};

template<class T>
void SyncTB<T>::tick(){
    // Execute a clock cycle and update variables
    // you need the eval at start and end, according to zipCPU
	dut->eval(); 
    dut->CLOCK_LINE = 1;
    dut->eval();
    dut->CLOCK_LINE = 0;
    dut->eval();
}

template<class T>
SyncTB<T>::SyncTB(int fClock, bool trace): traceOutput(trace), clockFrequency(fClock){
    dut = new T; // Assign the model the testbench manipulates
}
