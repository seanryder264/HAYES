import types_pkg::flags_t;

module log_mag_approx #(
    parameter int DATA_WIDTH = 16
)(
    input                               clk,
    input                               resetn,
    output                              ready_in,
    input                               ready_out,
    input flags_t                       flags_in,
    output flags_t                      flags_out,
    input logic signed [DATA_WIDTH-1:0] z_re, z_im,
    output logic [DATA_WIDTH-1:0]       log2_mag
);  

    localparam int NUM_STAGES = 3;

    typedef struct packed {
        flags_t flags;
    } meta_t;

    meta_t meta [0:NUM_STAGES-1];
    assign meta[0].flags = flags_in;
    assign ready_in = ready_out;

    localparam LOG2_MAG_WIDTH = $clog2(2 * DATA_WIDTH + 1);

    logic [2*DATA_WIDTH-1:0] z_re_sqr, z_im_sqr;
    logic [2*DATA_WIDTH:0] mag_sqr;
    logic [LOG2_MAG_WIDTH-1:0] msb_pos;

    /* verilator lint_off UNUSED */
    logic [2*DATA_WIDTH:0] norm_mag_full;
    /* verilator lint_on UNUSED */
    logic [DATA_WIDTH-1:0] norm_mag;

    logic [DATA_WIDTH-1:0] log2_lut [0:(2**DATA_WIDTH)-1];
    initial $readmemb("log2_lut.mem", log2_lut);
    
    always_ff @(posedge clk) begin
        if (!resetn) begin
            z_re_sqr <= 'b0;
            z_im_sqr <= 'b0;
            meta[1] <= 'b0;
        end else if (ready_out) begin
            z_re_sqr <= z_re * z_re;
            z_im_sqr <= z_im * z_im;
            meta[1] <= meta[0];
        end else begin
            z_re_sqr <= z_re_sqr;
            z_im_sqr <= z_im_sqr;
            meta[1] <= meta[1];
        end
    end // STAGE 1

    always_ff @(posedge clk) begin
        if (!resetn) begin
            mag_sqr <= 'b0;
            meta[2] <= 'b0;
        end else if (ready_out) begin
            mag_sqr <= z_re_sqr + z_im_sqr;
            meta[2] <= meta[1];
        end else begin
            mag_sqr <= mag_sqr;
            meta[2] <= meta[2];
        end
    end // STAGE 2

    integer i;
    always_comb begin
        msb_pos = '0;
        for (i = 2*DATA_WIDTH; i >= 0; i--) begin
            if (mag_sqr[i]) begin
                msb_pos = i[LOG2_MAG_WIDTH-1:0];
                break;
            end
        end

        norm_mag_full = (mag_sqr << DATA_WIDTH) >> msb_pos;
        norm_mag = norm_mag_full[DATA_WIDTH-1:0];
    end

    always_ff @(posedge clk) begin
        if (!resetn) begin
            log2_mag <= 'b0;
            flags_out <= 'b0;
        end else if (ready_out) begin
            log2_mag <= (norm_mag == 0) ? 'b0 : log2_lut[norm_mag];
            flags_out <= meta[2].flags;
        end else begin
            log2_mag <= log2_mag;
            flags_out <= flags_out;
        end
    end // STAGE 3

endmodule
