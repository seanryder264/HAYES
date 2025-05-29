module complex_sub(
    input [15:0]    a_re,
    input [15:0]    a_im,
    input [15:0]    b_re,
    input [15:0]    b_im,
    output [15:0]   c_re,
    output [15:0]   c_im
);

assign c_re = a_re - b_re;
assign c_im = a_im - b_im;

endmodule
