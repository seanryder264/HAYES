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

    reg [7:0] max, x;
    reg [7:0] r1, g1, b1;

    always @* begin
        max = 8'd255;
        case (hue / 43)  // 256 / 6 ≈ 43 per sector
            0: begin    // red -> yellow 
                x = (hue * 6);
                r1 = max;
                g1 = x;
                b1 = 8'd0;
            end
            1: begin    // yellow -> green
                x = (hue - 43) * 6;
                r1 = 255 - x;
                g1 = max;
                b1 = 8'd0;
            end
            2: begin    // green -> cyan
                x = (hue - 86) * 6;
                r1 = 8'd0;
                g1 = max;
                b1 = x;
            end
            3: begin    // cyan -> blue
                x = (hue - 129) * 6;
                r1 = 8'd0;
                g1 = 255 - x;
                b1 = max;
            end
            4: begin    // blue -> magenta
                x = (hue - 172) * 6;
                r1 = x;
                g1 = 8'd0;
                b1 = max;
            end
            default: begin  // magenta -> red
                x = (hue - 214) * 6;
                r1 = max;
                g1 = 8'd0;
                b1 = 255 - x;
            end
        endcase

        // Set RGB output
        red   = r1;
        green = g1;
        blue  = b1;
    end

endmodule


