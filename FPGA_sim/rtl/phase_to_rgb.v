module phase_to_rgb (
    input signed [15:0] phase, // Q1.15: -π to +π
    output reg [7:0] r, g, b
);

    // Convert [-π, π] → [0, 1535]
    wire signed [15:0] pi      = 16'sd25736; // π in Q1.15 ≈ 3.14159 * 2^15
    wire [15:0] norm_phase = (phase + pi) * 1536 / (2 * pi);

    reg [7:0] red, green, blue;

    always @(*) begin
        case (norm_phase[11:8]) // Divide into 6 sections of 256 steps
            4'd0: begin // Red → Yellow
                red   = 255;
                green = norm_phase[7:0];
                blue  = 0;
            end
            4'd1: begin // Yellow → Green
                red   = 255 - norm_phase[7:0];
                green = 255;
                blue  = 0;
            end
            4'd2: begin // Green → Cyan
                red   = 0;
                green = 255;
                blue  = norm_phase[7:0];
            end
            4'd3: begin // Cyan → Blue
                red   = 0;
                green = 255 - norm_phase[7:0];
                blue  = 255;
            end
            4'd4: begin // Blue → Magenta
                red   = norm_phase[7:0];
                green = 0;
                blue  = 255;
            end
            4'd5: begin // Magenta → Red
                red   = 255;
                green = 0;
                blue  = 255 - norm_phase[7:0];
            end
            default: begin
                red = 0; green = 0; blue = 0; // default black
            end
        endcase

        r = red;
        g = green;
        b = blue;
    end

endmodule