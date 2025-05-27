module mux4 (
    input  wire [1:0] sel,       
    input  wire [15:0] in0,      
    input  wire [15:0] in1,      
    input  wire [15:0] in2,      
    input  wire [15:0] in3,      
    output reg  [15:0] out
);

    always @(*) begin
        case (sel)
            2'b00: out = in0;
            2'b01: out = in1;
            2'b10: out = in2;
            2'b11: out = in3;
            default: out = 16'b0;
        endcase
    end

endmodule
