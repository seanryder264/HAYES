module atan_lut (
    input  signed [15:0] x, 
    input  signed [15:0] y,
    output reg [15:0]    angle
);

    // Absolute values
    wire [15:0] abs_x = (x[15]) ? (~x + 1) : x;
    wire [15:0] abs_y = (y[15]) ? (~y + 1) : y;

    // Opposite and adjacent
    wire [15:0] opp = (abs_x > abs_y) ? abs_y : abs_x;
    wire [15:0] adj = (abs_x > abs_y) ? abs_x : abs_y;

    // Ratio index generation: scaled to [0, 255]
    wire [23:0] ratio_mult = (opp << 8);     // Multiply by 256
    wire [15:0] ratio_index = (adj != 0) ? ratio_mult / adj : 0; // Avoid divide-by-zero

    // Reduced-size LUT: 256 entries, stored in LUTs
    reg [15:0] atan_lut [0:255];
    initial $readmemh("atan_lut_256.mem", atan_lut);

    wire [15:0] base_angle = atan_lut[ratio_index];

    // Quadrant correction
    always @* begin
        angle = 0;
        if (x >= 0 && y >= 0) begin // Q1
            angle = (abs_x >= abs_y) ? base_angle : 16'd16384 - base_angle ;
        end else if (x < 0 && y >= 0) begin // Q2
            angle = (abs_x >= abs_y) ? 16'd32768 - base_angle : 16'd16384 + base_angle;
        end else if (x < 0 && y < 0) begin // Q3
            angle = (abs_x >= abs_y) ? 16'd32768 + base_angle : 16'd49152 - base_angle;
        end else if (x >= 0 && y < 0) begin // Q4
            angle = (abs_x >= abs_y) ? 16'd0 - base_angle : 16'd49152 + base_angle;
        end
    end

endmodule
