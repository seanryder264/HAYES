module complex_sub(
    input signed [15:0]    a_re,
    input signed [15:0]    a_im,
    input signed [15:0]    b_re,
    input signed [15:0]    b_im,
    output signed [15:0]   c_re,
    output signed [15:0]   c_im
);

assign c_re = a_re - b_re;
assign c_im = a_im - b_im;

endmodule
