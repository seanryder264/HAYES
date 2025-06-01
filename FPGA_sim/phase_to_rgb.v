module phase_to_rgb (
    /* verilator lint_off UNUSED */
    input  wire [15:0] phase,  // 0 = -π, 65535 = π
    /* verilator lint_on UNUSED */
    output reg  [7:0] red,
    output reg  [7:0] green,
    output reg  [7:0] blue
);

    // Map phase to hue [0, 255]
    wire [7:0] hue = phase[15:8] + 8'd128;  // top 8 bits for hue

    reg [7:0] c, x;
    reg [7:0] r1, g1, b1;

    always @* begin
        c = 8'd255;
        case (hue / 43)  // 256 / 6 ≈ 43 per sector
            0: begin
                x = (hue * 6);
                r1 = c;
                g1 = x;
                b1 = 8'd0;
            end
            1: begin
                x = (43 * 2 - hue) * 6;
                r1 = x;
                g1 = c;
                b1 = 8'd0;
            end
            2: begin
                x = (hue - 86) * 6;
                r1 = 8'd0;
                g1 = c;
                b1 = x;
            end
            3: begin
                x = (129 - hue) * 6;
                r1 = 8'd0;
                g1 = x;
                b1 = c;
            end
            4: begin
                x = (hue - 172) * 6;
                r1 = x;
                g1 = 8'd0;
                b1 = c;
            end
            default: begin
                x = (215 - hue) * 6;
                r1 = c;
                g1 = 8'd0;
                b1 = x;
            end
        endcase

        // Set RGB output
        red   = r1;
        green = g1;
        blue  = b1;
    end

endmodule


