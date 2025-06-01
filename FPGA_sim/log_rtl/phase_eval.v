module phase_eval (
    input [63:0] zero_diff_re,  // {diff_re_3, diff_re_2, diff_re_1, diff_re_0}
    input [63:0] zero_diff_im,  // {diff_im_3, diff_im_2, diff_im_1, diff_im_0}
    input [63:0] pole_diff_re,  // {diff_re_3, diff_re_2, diff_re_1, diff_re_0}
    input [63:0] pole_diff_im,  // {diff_im_3, diff_im_2, diff_im_1, diff_im_0}
    output signed [15:0] phase_out
);

    // Unpack inputs
    wire signed [15:0] zero_diff_re_0 = zero_diff_re[15:0];
    wire signed [15:0] zero_diff_re_1 = zero_diff_re[31:16];
    wire signed [15:0] zero_diff_re_2 = zero_diff_re[47:32];
    wire signed [15:0] zero_diff_re_3 = zero_diff_re[63:48];
    
    wire signed [15:0] zero_diff_im_0 = zero_diff_im[15:0];
    wire signed [15:0] zero_diff_im_1 = zero_diff_im[31:16];
    wire signed [15:0] zero_diff_im_2 = zero_diff_im[47:32];
    wire signed [15:0] zero_diff_im_3 = zero_diff_im[63:48];
    
    wire signed [15:0] pole_diff_re_0 = pole_diff_re[15:0];
    wire signed [15:0] pole_diff_re_1 = pole_diff_re[31:16];
    wire signed [15:0] pole_diff_re_2 = pole_diff_re[47:32];
    wire signed [15:0] pole_diff_re_3 = pole_diff_re[63:48];
    
    wire signed [15:0] pole_diff_im_0 = pole_diff_im[15:0];
    wire signed [15:0] pole_diff_im_1 = pole_diff_im[31:16];
    wire signed [15:0] pole_diff_im_2 = pole_diff_im[47:32];
    wire signed [15:0] pole_diff_im_3 = pole_diff_im[63:48];

    // Internal phases
    wire signed [15:0] zero_phase_0, zero_phase_1, zero_phase_2, zero_phase_3;
    wire signed [15:0] pole_phase_0, pole_phase_1, pole_phase_2, pole_phase_3;

    // Instantiate atan_lut for zeros
    atan_lut atan_lut_z0 (.x(zero_diff_re_0), .y(zero_diff_im_0), .angle(zero_phase_0));
    atan_lut atan_lut_z1 (.x(zero_diff_re_1), .y(zero_diff_im_1), .angle(zero_phase_1));
    atan_lut atan_lut_z2 (.x(zero_diff_re_2), .y(zero_diff_im_2), .angle(zero_phase_2));
    atan_lut atan_lut_z3 (.x(zero_diff_re_3), .y(zero_diff_im_3), .angle(zero_phase_3));

    // Instantiate atan_lut for poles
    atan_lut atan_lut_p0 (.x(pole_diff_re_0), .y(pole_diff_im_0), .angle(pole_phase_0));
    atan_lut atan_lut_p1 (.x(pole_diff_re_1), .y(pole_diff_im_1), .angle(pole_phase_1));
    atan_lut atan_lut_p2 (.x(pole_diff_re_2), .y(pole_diff_im_2), .angle(pole_phase_2));
    atan_lut atan_lut_p3 (.x(pole_diff_re_3), .y(pole_diff_im_3), .angle(pole_phase_3));

    // Sum phases
    wire signed [18:0] zero_phase_sum = zero_phase_0 + zero_phase_1 + zero_phase_2 + zero_phase_3;
    wire signed [18:0] pole_phase_sum = pole_phase_0 + pole_phase_1 + pole_phase_2 + pole_phase_3;

    // Output phase
    assign phase_out = zero_phase_sum - pole_phase_sum;

endmodule
