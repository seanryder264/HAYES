module mul_complex  #(
    DATA_WIDTH = 16
) (
    input  wire [DATA_WIDTH:0] a_re,
    input  wire [DATA_WIDTH:0] a_im,
    input  wire [DATA_WIDTH:0] b_re,
    input  wire [DATA_WIDTH:0] b_im,
    output wire [DATA_WIDTH:0] mul_re,
    output wire [DATA_WIDTH:0] mul_im
);

assign mul_re = (a_re * b_re - a_im * b_im) >> 1; // Adjusted for fixed-point
assign mul_im = (a_re * b_im + a_im * b_re) >> 1; // Adjusted for fixed-point
    
endmodule
