module phase_lookup (
    input  signed [9:0] x_in,
    input  signed [9:0] y_in,
    output reg [7:0]    phase  // 0 to 255 = 0° to 360°
);

    reg [7:0] lut [0:255];  // 256-entry LUT for [0°, 45°] mapped to [0, 32]
    initial $readmemh("atan_lut.mem", lut);

    // Take absolute values of inputs (10-bit)
    wire [9:0] abs_x_in = x_in[9] ? -x_in : x_in;
    wire [9:0] abs_y_in = y_in[9] ? -y_in : y_in;

    // Determine max and min of abs_x and abs_y
    wire [9:0] max_val = (abs_x_in > abs_y_in) ? abs_x_in : abs_y_in;
    wire [9:0] min_val = (abs_x_in > abs_y_in) ? abs_y_in : abs_x_in;

    // Create fixed-point ratio: (min_val / max_val) in Q8.8
    wire [17:0] ratio_fixed = (max_val != 0) ? ((min_val << 8) / max_val) : 18'd0;
    wire [7:0]  lut_index   = ratio_fixed[15:8];

    wire [7:0] base_angle = lut[lut_index];

    // Determine final phase based on quadrant logic
    always @* begin
        if (x_in >= 0 && y_in >= 0) begin // Q1
            phase = (abs_x_in >= abs_y_in) ? base_angle : 8'd64 - base_angle;
        end else if (x_in < 0 && y_in >= 0) begin // Q2
            phase = (abs_x_in >= abs_y_in) ? 8'd128 - base_angle : 8'd64 + base_angle;
        end else if (x_in < 0 && y_in < 0) begin // Q3
            phase = (abs_x_in >= abs_y_in) ? 8'd128 + base_angle : 8'd192 - base_angle;
        end else begin // Q4
            phase = (abs_x_in >= abs_y_in) ? 8'd255 - base_angle : 8'd192 + base_angle;
        end
    end

endmodule


