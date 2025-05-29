module func_eval(
    input [15:0]    z_re,
    input [15:0]    z_im,
    output [15:0]   w_re,
    output [15:0]   w_im
);

reg [31:0] zeros [0:1];

assign zeros[0] = {16'd0, 16'd0};
assign zeros[1] = {-16'd0, -16'd0};

wire [15:0] a1_re, a1_im; 

complex_sub sub_one (   .a_re(z_re), .a_im(z_im),
                            .b_re(zeros[0][15:0]), .b_im(zeros[0][31:16]),
                            .c_re(a1_re), .c_im(a1_im));

wire [15:0] a2_re, a2_im; 

complex_sub sub_two (   .a_re(z_re), .a_im(z_im),
                            .b_re(zeros[1][15:0]), .b_im(zeros[1][31:16]),
                            .c_re(a2_re), .c_im(a2_im));       

complex_mult mult_one ( .a_re(a1_re), .a_im(a1_im),
                        .b_re(a2_re), .b_im(a2_im),
                        .c_re(w_re), .c_im(w_im));     

endmodule
