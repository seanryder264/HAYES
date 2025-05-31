module phase_eval (
    input signed [15:0] zero_diff_re [0:3],
    input signed [15:0] zero_diff_im [0:3],
    input signed [15:0] pole_diff_re [0:3],
    input signed [15:0] pole_diff_im [0:3],
    output signed [15:0] phase_out
);

    // Internal wires to hold the output phases of each LUT
    wire signed [15:0] zero_phase [0:3];
    wire signed [15:0] pole_phase [0:3];

    // Instantiate atan_lut for zeros
    atan_lut atan_lut_z0 (.x(zero_diff_re[0]), .y(zero_diff_im[0]), .angle(zero_phase[0]));
    atan_lut atan_lut_z1 (.x(zero_diff_re[1]), .y(zero_diff_im[1]), .angle(zero_phase[1]));
    atan_lut atan_lut_z2 (.x(zero_diff_re[2]), .y(zero_diff_im[2]), .angle(zero_phase[2]));
    atan_lut atan_lut_z3 (.x(zero_diff_re[3]), .y(zero_diff_im[3]), .angle(zero_phase[3]));

    // Instantiate atan_lut for poles
    atan_lut atan_lut_p0 (.x(pole_diff_re[0]), .y(pole_diff_im[0]), .angle(pole_phase[0]));
    atan_lut atan_lut_p1 (.x(pole_diff_re[1]), .y(pole_diff_im[1]), .angle(pole_phase[1]));
    atan_lut atan_lut_p2 (.x(pole_diff_re[2]), .y(pole_diff_im[2]), .angle(pole_phase[2]));
    atan_lut atan_lut_p3 (.x(pole_diff_re[3]), .y(pole_diff_im[3]), .angle(pole_phase[3]));

    // Sum zero phases and pole phases
    wire signed [18:0] zero_phase_sum;
    wire signed [18:0] pole_phase_sum;

    assign zero_phase_sum = zero_phase[0] + zero_phase[1] + zero_phase[2] + zero_phase[3];
    assign pole_phase_sum = pole_phase[0] + pole_phase[1] + pole_phase[2] + pole_phase[3];

    // Output phase = sum(zeros) - sum(poles)
    assign phase_out = zero_phase_sum - pole_phase_sum;

endmodule
