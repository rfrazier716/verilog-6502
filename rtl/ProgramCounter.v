
`default_nettype none //stops mispelled identifiers from being turned into wires
parameter PC_RESET_ADDR = 16'h0000; //Value the program counter gets reset to on 

module ProgramCounter(
    input wire sys_clock, // The system clock signal
    input wire reset, // Reset Signal
    input wire phase_1, phase_2, // The clock phases
    input wire pcl_pcl, adl_pcl, pch_pch,adh_pch, //signals that assert where to load the next PC address from, split for high and low
    input wire pcl_db, pcl_adl, pch_db,pch_adh, //signals that put the PC onto different IO buses
    input wire inc_pcl, //wire to assert that the next program counter address should the select_register + 1
    input wire increment_pc, //signal that asserts if the program counter is incremented or not

    inout wire[7:0] address_l, address_h, // Address buses, can be both driven and read from 
    output wire[7:0]  data_bus
);

    reg[15:0] program_counter; // the program counter is 2x 8 bit registers in the actual 6502 but is 1x16 bit register
    reg[15:0] program_counter_select; // program counter select register gets the next PC address to be loaded on phase 2
    initial program_counter = PC_RESET_ADDR; //initial value is the reset address
    initial program_counter_select = 0; //initial the PC select to zero

    //Synchronous Section
    always@(posedge sys_clock)
    begin
        //on a positive clock edge can be in either phase 1 or phase 2 of a cpu cycle
        if(reset)
            program_counter <= PC_RESET_ADDR; //reset the program counter address
        
        //in phase 1 the new address is selected
        if(phase_1) 
        begin
            //select the lower byte of the program counter
            if(pcl_pcl) program_counter_select[7:0]<=program_counter[7:0];
            if(pcl_adl) program_counter_select[7:0]<=address_l;
            
            //select the upper byte fo the program counter
            if(pch_pch) program_counter_select[15:8]<=program_counter[15:8];
            if(pch_adh) program_counter_select[15:8]<=address_h;
        end
        
        //in phase 2 we load the new PC address into memory
        if(phase_2)
        begin
            program_counter<=program_counter_select+{15'b0,inc_pcl}; //Load the next program counter value
        end
        
        
    end

    //Assert the PC onto the databus
    assign data_bus = pcl_db ? program_counter[7:0] : 8'hZZ; 
    assign data_bus = pch_db ? program_counter[15:8] : 8'hZZ;

    //Assert the PC onto the address buses
    assign address_l = pcl_adl ? program_counter[7:0] : 8'hZZ;
    assign address_h = pch_adh ? program_counter[15:8] : 8'hZZ;

endmodule