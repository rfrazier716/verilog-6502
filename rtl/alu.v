/*
 * 8-bit ALU implemented with wishbone pipeline bus
 * Registers:
 *  A - Accumulator 
 *  B - B input register
 *
 * Write Addresses:
 * 0x00 - Register A
 * 0x01 - Register B 
 * 
 * Read Addresses
 * 0x00 - Register A
 * 0x01 - Register B
 * 0x80 - Add - Returns A+B with flags
 * 0x81 - Add with Carry - Returns A&B with a_r carry input
 * More to be implemented when proved that this works out
*/





`default_nettype none //stops mispelled identifiers from being turned into wires

/* verilator lint_off UNUSED */
module alu(
    input wire i_clk, // System clock
    input wire reset, // Reset Signal
    input wire i_wb_stb, // Input_WishBone_Standby -- signal that is true for any bus transaction request
    input wire i_wb_we, // Input_WishBone_WriteEnable -- asserted when a_r write request takes place
    input wire[7:0] i_wb_addr, //Input_WishBone_Address -- the address of the request
    input wire[7:0] i_wb_data, //Input_WishBone_Data -- The Data to be written
    
    output wire o_wb_ack, //Output_WishBone_Acknowledge -- Slaves Response indicating request was acknowledged
    output wire o_wb_stall, //Output_WishBone_Stall -- controls flow of data to the slave, true when slave cannot accept a_r request
    output wire[7:0] o_wb_data //Output_WishBone_Data -- output line
);
    // ALU input/output and status registers
    reg[7:0] a_r, b_r, c_r;
    reg[7:0] flags, o_data_r; // internal registers 
    initial {a_r,a_r,c_r,flags,o_data_r} = 0; //initialize registers to zero

    // ALU implementation registers
    reg[7:0] add_r, addc_r;
    reg add_cout_r, addc_cout_r;

    reg overflow, carry_out, zero, negative; //uC status flags
    reg o_wb_ack_r; //acknowledge register

    /***********************************
     *
     * Wishbone communication section
     *
     ***********************************/

    //handling writes to slave
    //can update registers A or B, the operation register, and the flags register
    always@(posedge i_clk) begin
        if( i_wb_stb && i_wb_we && !o_wb_stall)
        case(i_wb_addr)
            8'h00: a_r <= i_wb_data;
            8'h01: b_r <= i_wb_data;
            default: begin end // do nothing by default
        endcase
    end

    //handling read requests
    always@(*) begin 
        case(i_wb_addr)
            8'h00: o_data_r = a_r; //put the a_register on the data bus register
            8'h01: o_data_r = b_r; //put the b_register on the data bus register
            8'h02: o_data_r = flags; //put the flags on the data bus register
            default: o_data_r = c_r; //otherwise the operation result sits on the data bus register
        endcase
    end

    //updating alu operation when associated register is read
    always@(posedge i_clk) begin
        flags = flags | {negative, overflow, 4'b0, zero, 0}; //always update the flags based on what's on the input and output
        if( i_wb_stb && (!o_wb_stall))
        case(i_wb_addr)
            8'h80: begin // add without carry in
                c_r <= add_r; //the last operation performed gets stored here so flags are properly set
                flags[0] <= add_cout_r; //put the carry out bit into the flags
            end

            8'h81: begin // add with a carry in
                c_r <= addc_r;
                flags[0] <= addc_cout_r; //put the carry bit onto flags
            end

            default : begin end // do nothing by default
        endcase
    end

    //only put the read requested data on the bus if it's a_r valid request
    assign o_wb_data = i_wb_stb ? o_data_r : 8'hZZ;
    assign o_wb_ack = o_wb_ack_r;

    //Setting the Acknowledge when the request is complete
    always@(posedge i_clk) begin
        o_wb_ack_r <= i_wb_stb && !o_wb_stall;
    end

    //all our operations happen on a single clock 
    //Should never have a case where we cannot accept input
    assign o_wb_stall = 1'b0; 



    // ALU Operation
    always@(*) begin
        {add_cout_r, add_r} = a_r + b_r; // an addition operation effects the carry out bit
        {addc_cout_r, addc_r} = a_r+ b_r + 8'b1; //addition with carry in
    end

    // ALU Flags
    assign negative = c_r[7]; //Negative is the MSB of the output
    assign zero = |c_r; // zero is the bitwise or of the 
    assign overflow = ( a_r[7] && b_r[7] && (!c_r[7])) || ((!a_r[7]) && (!b_r[7]) && c_r[7]); //overflow if output is different sign than inputs

endmodule