module colour_app (
    /* verilator lint_off UNUSED */
    input  wire [7:0] phase,  // 0 = -π, 65535 = π
    input  wire [7:0] log_mag,
    /* verilator lint_on UNUSED */
    output reg  [7:0] red,
    output reg  [7:0] green,
    output reg  [7:0] blue
);

    // Map phase to hue [0, 255]
    wire [7:0] hue = phase + 8'd128;

    reg [7:0] max, x;
    reg [7:0] r1, g1, b1;
    reg [7:0] brightness; // reg now, since it's assigned inside always

    /* verilator lint_off UNUSED */
    wire [15:0] r2 = r1 * brightness;
    wire [15:0] g2 = g1 * brightness;
    wire [15:0] b2 = b1 * brightness;
    /* verilator lint_on UNUSED */

    always @* begin

        brightness = log_mag[7:0];

        max = 8'd255;
        case (hue / 43)
            0: begin
                x = hue * 6;
                r1 = max;
                g1 = x;
                b1 = 8'd0;
            end
            1: begin
                x = (hue - 43) * 6;
                r1 = max - x;
                g1 = max;
                b1 = 8'd0;
            end
            2: begin
                x = (hue - 86) * 6;
                r1 = 8'd0;
                g1 = max;
                b1 = x;
            end
            3: begin
                x = (hue - 129) * 6;
                r1 = 8'd0;
                g1 = max - x;
                b1 = max;
            end
            4: begin
                x = (hue - 172) * 6;
                r1 = x;
                g1 = 8'd0;
                b1 = max;
            end
            default: begin
                x = (hue - 214) * 6;
                r1 = max;
                g1 = 8'd0;
                b1 = max - x;
            end
        endcase

        // Apply brightness scaling: multiply then divide by 255
        red   = r2[15:8] ;  // Same as / 256 (approx for /255)
        green = g2[15:8] ;
        blue  = b2[15:8] ;
    end

endmodule



