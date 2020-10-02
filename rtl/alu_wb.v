`default_nettype none //stops mispelled identifiers from being turned into wires


module alu_wb(
    input wire i_clk, // System clock
    input wire reset, // Reset Signal
    input wire i_wb_stb, // Input_WishBone_Standby -- signal that is true for any bus transaction request
    input wire i_wb_we, // Input_WishBone_WriteEnable -- asserted when a write request takes place
    input wire[3:0] i_wb_addr, //Input_WishBone_Address -- the address of the request
    input wire[7:0] i_wb_data, //Input_WishBone_Data -- The Data to be written
    
    output wire o_wb_ack, //Output_WishBone_Acknowledge -- Slaves Response indicating request was acknowledged
    output wire o_wb_stall, //Output_WishBone_Stall -- controls flow of data to the slave, true when slave cannot accept a request
    output wire o_wb_data, //Output_WishBone_Data -- output line
);
    reg[7:0] a, b, operation, out, flags, o_data; // internal registers 
    initial a, b, out, flags, operation = 8'b0; // initial values for all registers is zero

    //handling writes to slave
    //can update registers A or B, the operation register, and the flags register
    always@(posedge i_clk) begin
        if( i_wb_stb && i_wb_we && !o_wb_stall)
        case(i_wb_addr)
            8'h80: a <= i_wb_data
            8'h81: b <= i_wb_data
            8'h82: operation <= i_wb_data
            8'h84: flags <= o_data
            default: begin end // do nothing by default
        endcase
    end

    //handling read requests
    always@(posedge i_clk) begin
        if( i_wb_stb && (!i_wb_we) && (!o_wb_stall))
        case(i_wb_addr)
            8'h80: o_data <= a;
            8'h81: o_data <= b;
            8'h82: o_data <= operation;
            8'h83: o_data <= out;
            8'h84: o_data <= flags;
            default : begin end // do nothing by default
    end

    //only put the read requested data on the bus if it's a valid request
    assign o_wb_data = i_wb_stb ? o_data : 8'hZZ;

    //Setting the Acknowledge when the request is complete
    always@(posedge i_clk) begin
        o_wb_ack <= i_wb_stb && !o_wb_stall
    end

    //all our operations happen on a single clock 
    //Should never have a case where we cannot accept input
    assign o_wb_stall = 1'b0; 

endmodule