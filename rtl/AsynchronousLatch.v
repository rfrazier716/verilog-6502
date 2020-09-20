//pipelined system used to convert asynchronous latch signals into synchronous signals
//for a signal, if it goes high, the rising_edge will be high for the next clock cycle
// when it goes low, falling edge will be high for the next clock cycle
// Enables synthesis of old, slower, hardware emulation without clock lines being used to route latch signals


`default_nettype none //stops mispelled identifiers from being turned into wires

module AsynchronousLatch(
    input wire sys_clock, //The system Clock
    input wire reset, //Reset signal
    input wire async_signal, // the asynchronous signal you want to catch rising and falling edges for
    output wire rising_edge, falling_edge //the Rising and Falling edge output signals 
);

    reg[2:0] shift_register;
    initial shift_register = 0; 

    always@(posedge sys_clock)
    begin
        if(reset) 
            shift_register <= 3'b0;
        else
            shift_register <= (shift_register >> 1) + {async_signal,2'b0}; //shift out the lowest bit and add the input signal to the highest
    end

    assign rising_edge = (^shift_register[1:0]) & shift_register[1]; //Rising edge when the 2LSBs differ and bit 1 is high
    assign falling_edge = (^shift_register[1:0]) & shift_register[0]; //Falling edge when 2LSBs differ and bit 0 is high
endmodule