module func_eval(
    input [15:0]    z_re,
    input [15:0]    z_im,
    output [15:0]   w_re,
    output [15:0]   w_im
);

reg [31:0] zeros [0:1];
// reg [31:0] poles [0:1];

assign zeros[0] = {16'd10, 16'd10};
assign zeros[1] = {16'd0, 16'd0};

// assign poles[0] = {-16'd10, -16'd10};
// assign poles[1] = {16'd0, 16'd0};


wire [15:0] a1_re, a1_im; 

complex_sub sub_1 (   .a_re(z_re), .a_im(z_im),
                            .b_re(zeros[0][15:0]), .b_im(zeros[0][31:16]),
                            .c_re(a1_re), .c_im(a1_im));

wire [15:0] a2_re, a2_im; 

complex_sub sub_2 (   .a_re(z_re), .a_im(z_im),
                            .b_re(zeros[1][15:0]), .b_im(zeros[1][31:16]),
                            .c_re(a2_re), .c_im(a2_im));       

// wire [15:0] m1_re, m1_im;

complex_mult mult_1 ( .a_re(a1_re), .a_im(a1_im),
                        .b_re(a2_re), .b_im(a2_im),
                        .c_re(w_re), .c_im(w_im));     

// wire [15:0] a3_re, a3_im; 

// complex_sub sub_3 (   .a_re(z_re), .a_im(z_im),
//                             .b_re(poles[0][15:0]), .b_im(poles[0][31:16]),
//                             .c_re(a3_re), .c_im(a3_im));

// wire [15:0] a4_re, a4_im; 

// complex_sub sub_4 (   .a_re(z_re), .a_im(z_im),
//                             .b_re(poles[1][15:0]), .b_im(poles[1][31:16]),
//                             .c_re(a4_re), .c_im(a4_im));  

// wire [15:0] m2_re, m2_im;

// complex_mult mult_2 ( .a_re(a3_re), .a_im(a3_im),
//                         .b_re(a4_re), .b_im(a4_im),
//                         .c_re(m2_re), .c_im(m2_im)); 

// complex_div div (.a_re(m1_re), .a_im(m1_im),
//                         .b_re(m2_re), .b_im(m2_im),
//                         .c_re(w_re), .c_im(w_im));

endmodule
