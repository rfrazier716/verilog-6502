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
 * 0x81 - Add with Carry - Returns A&B with a carry input
 * More to be implemented when proved that this works out
*/





`default_nettype none //stops mispelled identifiers from being turned into wires

/* verilator lint_off UNUSED */
module alu(
    input wire i_clk, // System clock
    input wire reset, // Reset Signal
    input wire i_wb_stb, // Input_WishBone_Standby -- signal that is true for any bus transaction request
    input wire i_wb_we, // Input_WishBone_WriteEnable -- asserted when a write request takes place
    input wire[7:0] i_wb_addr, //Input_WishBone_Address -- the address of the request
    input wire[7:0] i_wb_data, //Input_WishBone_Data -- The Data to be written
    
    output wire o_wb_ack, //Output_WishBone_Acknowledge -- Slaves Response indicating request was acknowledged
    output wire o_wb_stall, //Output_WishBone_Stall -- controls flow of data to the slave, true when slave cannot accept a request
    output wire[7:0] o_wb_data //Output_WishBone_Data -- output line
);
    reg[7:0] a, b, o_data; // internal registers 
    reg overflow, carry_out, zero, negative; //uC status flags
    initial {a, b, o_data, overflow, carry_out, zero, negative} = 0; // initial values for all registers is zero

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
            8'h00: a <= i_wb_data;
            8'h01: b <= i_wb_data;
            default: begin end // do nothing by default
        endcase
    end

    //handling read requests
    always@(posedge i_clk) begin
        if( i_wb_stb && (!o_wb_stall))
        case(i_wb_addr)
            8'h00: o_data <= a; // put register a onto the data bus
            8'h01: o_data <= b; // put register b onto the data bus
            default : begin end // do nothing by default
        endcase
    end

    //only put the read requested data on the bus if it's a valid request
    assign o_wb_data = i_wb_stb ? o_data : 8'hZZ;

    //Setting the Acknowledge when the request is complete
    always@(posedge i_clk) begin
        o_wb_ack <= i_wb_stb && !o_wb_stall;
    end

    //all our operations happen on a single clock 
    //Should never have a case where we cannot accept input
    assign o_wb_stall = 1'b0; 
endmodule