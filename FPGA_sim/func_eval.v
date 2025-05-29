module func_eval(
    input [15:0]    z_re,
    input [15:0]    z_im,
    output [15:0]   w_re,
    output [15:0]   w_im
);

assign w_re = z_re;
assign w_im = z_im;

endmodule
