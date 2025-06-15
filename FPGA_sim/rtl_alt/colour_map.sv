import types_pkg::flags_t;

module colour_map (
    input               clk,
    input               resetn,

    input  [10:0]       hue,            // 0–1535, representing hue angle
    input  [8:0]        log_mag,        // brightness
    input  flags_t      flags_in,
    output              in_ready,

    output logic [7:0]  r, g, b,
    output flags_t      flags_out,
    input               out_ready
);  

    typedef enum logic [2:0] {
        RED_YELLOW      = 3'd0,
        YELLOW_GREEN    = 3'd1,
        GREEN_CYAN      = 3'd2,
        CYAN_BLUE       = 3'd3,
        BLUE_MAGENTA    = 3'd4,
        MAGENTA_RED     = 3'd5
    } hue_region_t;

    logic [7:0] r_hue, g_hue, b_hue;
    logic [7:0] mod_hue_8 = hue[7:0];
    logic [7:0] darkness = log_mag[8] ? log_mag[7:0] : 8'd0;
    hue_region_t hue_region = hue[10:8];

    always_comb begin
        case (hue_region)
            RED_YELLOW    : begin r_hue = 8'd255;        g_hue = increasing;    b_hue = 8'd0;         end
            YELLOW_GREEN  : begin r_hue = decreasing;    g_hue = 8'd255;        b_hue = 8'd0;         end
            GREEN_CYAN    : begin r_hue = 8'd0;          g_hue = 8'd255;        b_hue = increasing;   end
            CYAN_BLUE     : begin r_hue = 8'd0;          g_hue = decreasing;    b_hue = 8'd255;       end
            BLUE_MAGENTA  : begin r_hue = increasing;    g_hue = 8'd0;          b_hue = 8'd255;       end
            MAGENTA_RED   : begin r_hue = 8'd255;        g_hue = 8'd0;          b_hue = decreasing;   end
            default       : begin r_hue = 8'd255;        g_hue = 8'd255;        b_hue = 8'd255;       end
        endcase
    end

    always_ff @(posedge clk) begin
        if (!resetn || hue > 11'd1535) begin
            r <= 8'd255;
            g <= 8'd255;
            b <= 8'd255;
            flags_out <= 'b0;
        end else if (out_ready) begin
            r <= r_hue - darkness;
            g <= g_hue - darkness;
            b <= b_hue - darkness;
            flags_out <= flags_in;
        end else begin
            r <= r;
            g <= g;
            b <= b;
            flags_out <= flags_out;
        end
    end

    assign in_ready = out_ready;

endmodule
