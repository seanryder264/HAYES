module colour_app (
    input clk,
    input resetn,
    input ready,

    input wire signed [15:0] phase,
    input wire signed [7:0]  log_mag,

    output reg [7:0] red,
    output reg [7:0] green,
    output reg [7:0] blue
);

    // =================================================================
    // Pipeline 1: Calculate Hue and Sector from 16-bit phase
    // =================================================================
    wire [15:0] hue_s1 = phase;
    /* verilator lint_off WIDTH */
    wire [2:0] sector_s1 = phase / 16'd10922;
    /* verilator lint_on WIDTH */

    // Pipeline registers for stage 2
    reg [15:0] hue_s2;
    reg [2:0]  sector_s2;

    // =================================================================
    // Pipeline 2: Calculate full-brightness RGB based on hue
    // =================================================================
    reg [15:0] r1_s2, g1_s2, b1_s2;
    wire [15:0] interp_val;
    wire [15:0] interp_val = (hue_s2 % (43 * 256)) * 6;

    always @* begin
        r1_s2 = 0; g1_s2 = 0; b1_s2 = 0;

        case (sector_s2)
            0: begin r1_s2 = 16'hFFFF; g1_s2 = interp_val; b1_s2 = 16'h0000; end
            1: begin r1_s2 = 16'hFFFF - interp_val; g1_s2 = 16'hFFFF; b1_s2 = 16'h0000; end
            2: begin r1_s2 = 16'h0000; g1_s2 = 16'hFFFF; b1_s2 = interp_val; end
            3: begin r1_s2 = 16'h0000; g1_s2 = 16'hFFFF - interp_val; b1_s2 = 16'hFFFF; end
            4: begin r1_s2 = interp_val; g1_s2 = 16'h0000; b1_s2 = 16'hFFFF; end
            default: begin r1_s2 = 16'hFFFF; g1_s2 = 16'h0000; b1_s2 = 16'hFFFF - interp_val; end
        endcase
    end

    // Pipeline registers for stage 3
    reg [15:0] r1_s3;
    reg [15:0] g1_s3;
    reg [15:0] b1_s3;
    reg [7:0]  brightness_s3; // Changed to 8 bits

    // =================================================================
    // Pipeline 3: Apply brightness and scale to final 8-bit color
    // =================================================================
    // Product of 16-bit color and 8-bit brightness is 24 bits
   
    /* verilator lint_off UNUSED */
    wire [23:0] r2_s3 = r1_s3 * brightness_s3;
    wire [23:0] g2_s3 = g1_s3 * brightness_s3;
    wire [23:0] b2_s3 = b1_s3 * brightness_s3;
    /* verilator lint_on UNUSED */

    // Control Logic
    always @(posedge clk) begin
        if (!resetn) begin
            red   <= 8'd0;
            green <= 8'd0;
            blue  <= 8'd0;
        end else if (ready) begin
            // Stage 1 -> Stage 2
            hue_s2      <= hue_s1;
            sector_s2   <= sector_s1;

            // Stage 2 -> Stage 3
            r1_s3         <= r1_s2;
            g1_s3         <= g1_s2;
            b1_s3         <= b1_s2;
            brightness_s3 <= log_mag;

            // Stage 3 -> Output
            // Scale the 24-bit product down to 8 bits by taking the top 8 bits.
            // This is equivalent to (r1 * brightness) / 65536
            red   <= r2_s3[23:16];
            green <= g2_s3[23:16];
            blue  <= b2_s3[23:16];
        end
    end

endmodule
