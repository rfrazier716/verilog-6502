//6502 ALU Model
// Does not implement Decimal Mode

`default_nettype none //stops mispelled identifiers from being turned into wires

module alu(
    input wire ADD_en, AND_en, XOR_en, OR_en, SR_en, //ALU Operations
    input wire[7:0] A_in, B_in, //input registers for operation
    input wire Carry_in, //Carry in bit
    output wire OVFLW, Carry_out, //Overflow, carry out, and half-carry output bits
    output wire[7:0] C_out // Result of the ALU operation
);

    //Combinatorial logic block that performs ALU operations
    always@(*)
        begin
            // Default case for Carry_out incase it is not assigned
            Carry_out = 0;

            // Apply ALU Operation and put onto C_out
            case({ADD_en, AND_en, XOR_en, OR_en, SR_en})
                5'b00001: {C_out, Carry_out} = {1'b0,A_in}; //Shift Right operation
                5'b00010: C_out = A_in | B_in; // Bitwise Or Operation
                5'b00100: C_out = A_in ^ B_in; // Exclusive Or Operation
                5'b01000: C_out = A_in & B_in; // Bitwise AND Operation
                default:  {Carry_out,C_out} = A_in + B_in + {7'b0, Carry_in}; //Default operation is an Add                
            endcase
        end

        //Set overflow -- if two signed numbers produce a result with a different sign
        assign OVFLW = ( A_in[7] && B_in[7] && (!C_out[7])) || ((!A_in[7]) && (!B_in[7]) && C_out[7]); 
endmodule