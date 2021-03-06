
`default_nettype none //stops mispelled identifiers from being turned into wires

 module ProgramCounter#(parameter PC_RESET_ADDR =16'h0000) (
    input wire sys_clock, // The system clock signal
    input wire reset, // Reset Signal
    input wire phase_2_rising, //Signal asserts itself for one clock cycle on the rising edge of the phase 2 clock
    input wire pcl_pcl, adl_pcl, pch_pch,adh_pch, //signals that assert where to load the next PC address from, split for high and low
    input wire pcl_db, pcl_adl, pch_db,pch_adh, //signals that put the PC onto different IO buses
    input wire increment_pc, //signal that asserts if the program counter is incremented or not

    inout wire[7:0] address_l, address_h, // Address buses, can be both driven and read from 
    output wire[7:0]  data_bus
);

    reg[15:0] program_counter; // the program counter is 2x 8 bit registers in the actual 6502 but is 1x16 bit register
    reg[15:0] program_counter_select; // program counter select register gets the next PC address to be loaded on phase 2
    initial program_counter = PC_RESET_ADDR; //initial value is the reset address
    initial program_counter_select = 0; //initial the PC select to zero
    
    //Set up synchronous rising edge outputs for pcl_pcl, adl_pcl, pch_pch, and adh_pch
    wire pcl_pcl_r, adl_pcl_r, pch_pch_r, adh_pch_r;

    /* verilator lint_off PINMISSING */
    AsynchronousLatch loadFlags[3:0](
        .sys_clock(sys_clock),
        .reset(reset),
        .async_signal({pcl_pcl,adl_pcl,pch_pch,adh_pch}),
        .rising_edge({pcl_pcl_r,adl_pcl_r,pch_pch_r,adh_pch_r})
    );
    /* verilator lint_on PINMISSING */

    //Synchronous Section
    always@(posedge sys_clock)
    begin
        if(reset)
            begin
                program_counter <= PC_RESET_ADDR; //reset the program counter address
                program_counter_select <= PC_RESET_ADDR;
            end
        
        else begin
            //Check if a rising edge occured on any of the program_counter_high latches
            if(pch_pch_r)       program_counter_select[15:8]<=program_counter[15:8];
            else if(adh_pch_r)  program_counter_select[15:8]<=address_h;
            else                program_counter_select[15:8]<=program_counter_select[15:8];
            
            //Check if a rising edge occured on any of the program_counter_low latches
            if(pcl_pcl_r)       program_counter_select[7:0]<=program_counter[7:0];
            else if(adl_pcl_r)  program_counter_select[7:0]<=address_l;
            else                program_counter_select[7:0]<=program_counter_select[7:0];

            //in phase 2 we load the new PC address into memory
            if(phase_2_rising)
                program_counter<=program_counter_select+{15'b0,increment_pc}; //Load the next program counter value     
        end
    end

    //Assert the PC onto the databus
    assign data_bus = pch_db ? program_counter[15:8] : 8'hZZ;
    assign data_bus = pcl_db ? program_counter[7:0] : 8'hZZ; 
    
    //Assert the PC onto the address buses
    assign address_h = pch_adh ? program_counter[15:8] : 8'hZZ;
    assign address_l = pcl_adl ? program_counter[7:0] : 8'hZZ;
    

endmodule