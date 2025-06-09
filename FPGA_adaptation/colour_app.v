module colour_app (
    input clk,
    input resetn,
    input ready,

    /* verilator lint_off UNUSED */
    input  wire [7:0] phase,  // 0 = -π, 65535 = π
    input  wire [7:0] log_mag,
    /* verilator lint_on UNUSED */
    output reg  [7:0] red,
    output reg  [7:0] green,
    output reg  [7:0] blue
);

    //Pipeline 1
    wire [7:0] hue_s1    = phase + 8'd128;
    wire [7:0] sector_s1 = hue_s1 / 43;

    reg [7:0]  hue_s2;
    reg [7:0]  sector_s2;
    reg [7:0]  log_mag_s2;

    //Pipeline 2
    reg [7:0] r1_s2, g1_s2, b1_s2;
    reg [7:0] x_s2;

    always @* begin
        // Default assignments
        r1_s2 = 0; g1_s2 = 0; b1_s2 = 0; x_s2 = 0;

        case (sector_s2)
            0: begin
                x_s2  = hue_s2 * 6;
                r1_s2 = 8'd255;
                g1_s2 = x_s2;
                b1_s2 = 8'd0;
            end
            1: begin
                x_s2  = (hue_s2 - 43) * 6;
                r1_s2 = 8'd255 - x_s2;
                g1_s2 = 8'd255;
                b1_s2 = 8'd0;
            end
            2: begin
                x_s2  = (hue_s2 - 86) * 6;
                r1_s2 = 8'd0;
                g1_s2 = 8'd255;
                b1_s2 = x_s2;
            end
            3: begin
                x_s2  = (hue_s2 - 129) * 6;
                r1_s2 = 8'd0;
                g1_s2 = 8'd255 - x_s2;
                b1_s2 = 8'd255;
            end
            4: begin
                x_s2  = (hue_s2 - 172) * 6;
                r1_s2 = x_s2;
                g1_s2 = 8'd0;
                b1_s2 = 8'd255;
            end
            default: begin // 5
                x_s2  = (hue_s2 - 214) * 6;
                r1_s2 = 8'd255;
                g1_s2 = 8'd0;
                b1_s2 = 8'd255 - x_s2;
            end
        endcase
    end

    reg [7:0]  r1_s3;
    reg [7:0]  g1_s3;
    reg [7:0]  b1_s3;
    reg [7:0]  brightness_s3; // Renamed from log_mag

    /* verilator lint_off UNUSED */
    wire [15:0] r2_s3 = r1_s3 * brightness_s3;
    wire [15:0] g2_s3 = g1_s3 * brightness_s3;
    wire [15:0] b2_s3 = b1_s3 * brightness_s3;
    /* verilator lint_on UNUSED */

    //Control Logic
    always @(posedge clk) begin
        if (!resetn) begin
            // Reset all pipeline registers and outputs
            red         <= 8'd0;
            green       <= 8'd0;
            blue        <= 8'd0;
        end 
        else if (ready) begin
            hue_s2      <= hue_s1;
            sector_s2   <= sector_s1;
            log_mag_s2  <= log_mag;
            
            r1_s3         <= r1_s2;
            g1_s3         <= g1_s2;
            b1_s3         <= b1_s2;
            brightness_s3 <= log_mag_s2;

            red         <= r2_s3[15:8]; //Division by 255
            green       <= g2_s3[15:8];
            blue        <= b2_s3[15:8];
        end
    end

endmodule
