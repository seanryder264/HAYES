module complex_sub (
    input  signed [15:0] x,
    input  signed [15:0] y,

    input  [31:0] zero_0, zero_1, zero_2, zero_3,
    input  [31:0] pole_0, pole_1, pole_2, pole_3,

    output signed [15:0] zero_diff_re_0, zero_diff_re_1, zero_diff_re_2, zero_diff_re_3,
    output signed [15:0] zero_diff_im_0, zero_diff_im_1, zero_diff_im_2, zero_diff_im_3,
    output signed [15:0] pole_diff_re_0, pole_diff_re_1, pole_diff_re_2, pole_diff_re_3,
    output signed [15:0] pole_diff_im_0, pole_diff_im_1, pole_diff_im_2, pole_diff_im_3
);

    // Subtract each zero: z - zero[i]
    assign zero_diff_re_0 = x - $signed(zero_0[31:16]);
    assign zero_diff_im_0 = y - $signed(zero_0[15:0]);
    assign zero_diff_re_1 = x - $signed(zero_1[31:16]);
    assign zero_diff_im_1 = y - $signed(zero_1[15:0]);
    assign zero_diff_re_2 = x - $signed(zero_2[31:16]);
    assign zero_diff_im_2 = y - $signed(zero_2[15:0]);
    assign zero_diff_re_3 = x - $signed(zero_3[31:16]);
    assign zero_diff_im_3 = y - $signed(zero_3[15:0]);

    // Subtract each pole: z - pole[i]
    assign pole_diff_re_0 = x - $signed(pole_0[31:16]);
    assign pole_diff_im_0 = y - $signed(pole_0[15:0]);
    assign pole_diff_re_1 = x - $signed(pole_1[31:16]);
    assign pole_diff_im_1 = y - $signed(pole_1[15:0]);
    assign pole_diff_re_2 = x - $signed(pole_2[31:16]);
    assign pole_diff_im_2 = y - $signed(pole_2[15:0]);
    assign pole_diff_re_3 = x - $signed(pole_3[31:16]);
    assign pole_diff_im_3 = y - $signed(pole_3[15:0]);

endmodule
