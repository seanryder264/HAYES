import types_pkg::flags_t;

module pixel_generator(
input           clk,
input           s_axi_lite_aclk,
input           axi_resetn,
input           periph_resetn,

//Stream output
output [31:0]   out_stream_tdata,
output [3:0]    out_stream_tkeep,
output          out_stream_tlast,
input           out_stream_tready,
output          out_stream_tvalid,
output [0:0]    out_stream_tuser,

//AXI-Lite S

/* verilator lint_off UNUSED */
input [AXI_LITE_ADDR_WIDTH-1:0]     s_axi_lite_araddr,
output                              s_axi_lite_arready,
input                               s_axi_lite_arvalid,

input [AXI_LITE_ADDR_WIDTH-1:0]     s_axi_lite_awaddr,
output                              s_axi_lite_awready,
input                               s_axi_lite_awvalid,
/* verilator lint_on UNUSED */

input                       s_axi_lite_bready,
output [1:0]                s_axi_lite_bresp,
output                      s_axi_lite_bvalid,

output [DATA_WIDTH-1:0]     s_axi_lite_rdata,
input                       s_axi_lite_rready,
output [1:0]                s_axi_lite_rresp,
output                      s_axi_lite_rvalid,

input  [DATA_WIDTH-1:0]     s_axi_lite_wdata,
output                      s_axi_lite_wready,
input                       s_axi_lite_wvalid,

output [7:0] r, g, b,
output logic colour_flags_valid,
output logic pixel_packer_ready

);

// keep to powers of 2
localparam TOTAL_POLES_ZEROS = 16;
localparam AXI_LITE_ADDR_WIDTH = 8;
localparam DATA_WIDTH = 16;
localparam X_SIZE = 1024;
localparam Y_SIZE = 1024;

localparam MAX_POLES_ZEROS = TOTAL_POLES_ZEROS / 2;
localparam logic [5:0] REG_FILE_SIZE = 2 * TOTAL_POLES_ZEROS + 1;
localparam HALF_WIDTH = DATA_WIDTH / 2;
localparam PZ_FIELD_WIDTH = $clog2(MAX_POLES_ZEROS) + 1;

logic [DATA_WIDTH - 1:0] regfile [0:REG_FILE_SIZE-1];

data_mem #( 
    .DATA_WIDTH(DATA_WIDTH),
    .REG_FILE_SIZE(REG_FILE_SIZE),
    .AXI_LITE_ADDR_WIDTH(AXI_LITE_ADDR_WIDTH)
) data_mem_inst (  
    .aclk(s_axi_lite_aclk), .resetn(axi_resetn),
    .regfile(regfile),
    .araddr(s_axi_lite_araddr), .arready(s_axi_lite_arready), .arvalid(s_axi_lite_arvalid),
    .awaddr(s_axi_lite_awaddr), .awready(s_axi_lite_awready), .awvalid(s_axi_lite_awvalid),
    .bready(s_axi_lite_bready), .bresp(s_axi_lite_bresp), .bvalid(s_axi_lite_bvalid),
    .rdata(s_axi_lite_rdata), .rready(s_axi_lite_rready), .rresp(s_axi_lite_rresp), .rvalid(s_axi_lite_rvalid),
    .wdata(s_axi_lite_wdata), .wready(s_axi_lite_wready), .wvalid(s_axi_lite_wvalid)
);

// always_comb begin
//     $display("%d", regfile[0]);
// end

logic signed [DATA_WIDTH-1:0] coord_re, coord_im;
flags_t coord_gen_flags;
logic latch_ready, frame_done;

coordinate_generator #(
    .DATA_WIDTH(DATA_WIDTH),
    .X_SIZE(X_SIZE), .Y_SIZE(Y_SIZE) 
) coordinate_gen (  
    .clk(clk), .resetn(periph_resetn), .ready(latch_ready),
    .x(coord_re), .y(coord_im), 
    .done(frame_done),
    .valid(coord_gen_flags.valid), .sof(coord_gen_flags.sof), .eol(coord_gen_flags.eol)
); // 1 STAGE

// always_comb begin
//     $display("%d %d", coord_re, coord_im);
// end

logic [DATA_WIDTH-1:0] latched_regfile [0:REG_FILE_SIZE-1];
logic function_eval_ready;

regfile_latch #(
    .DATA_WIDTH(DATA_WIDTH),
    .REG_FILE_SIZE(REG_FILE_SIZE)
) regfile_latch_inst (
    .clk(clk),
    .resetn(periph_resetn),
    .latch(frame_done),
    .ready(function_eval_ready),
    .data_in(regfile),
    .data_out(latched_regfile),
    .latch_ready(latch_ready)
);  // 1 STAGE

// always_comb begin
//     $display("%d", latched_regfile[0]);
// end

logic signed [DATA_WIDTH-1:0] pz_re [0:TOTAL_POLES_ZEROS-1];
logic signed [DATA_WIDTH-1:0] pz_im [0:TOTAL_POLES_ZEROS-1];
logic [PZ_FIELD_WIDTH-1:0] num_zeros;
logic [PZ_FIELD_WIDTH-1:0] num_poles;

assign num_zeros = latched_regfile[0][HALF_WIDTH +: PZ_FIELD_WIDTH];
assign num_poles = latched_regfile[0][0 +: PZ_FIELD_WIDTH];

// always_comb begin
//     $display("%d %d", num_zeros, num_poles);
// end

genvar i;
generate
    for (i = 0; i < TOTAL_POLES_ZEROS; i++) begin
        assign pz_re[i] = latched_regfile[2*i+1];
        assign pz_im[i] = latched_regfile[2*i+2];

        // always_comb begin
        //     $display("%d %d", pz_re[0], pz_im[0]);
        // end
    end
endgenerate

wire signed [DATA_WIDTH-1:0] diff_re [0:TOTAL_POLES_ZEROS-1];
wire signed [DATA_WIDTH-1:0] diff_im [0:TOTAL_POLES_ZEROS-1];

genvar j;
generate 
    for (j = 0; j < TOTAL_POLES_ZEROS; j++) begin
        complex_sub complex_sub_inst_i (
            .a_re(coord_re), .a_im(coord_im),
            .b_re(pz_re[j]), .b_im(pz_im[j]),
            .c_re(diff_re[j]), .c_im(diff_im[j])
        ); // 0 STAGE
    end
endgenerate

// always_comb begin
//     $display("%d %d", diff_re[0], diff_im[0]);
// end

logic [DATA_WIDTH-1:0] phases [0:TOTAL_POLES_ZEROS-1];
logic [DATA_WIDTH-1:0] log_mags [0:TOTAL_POLES_ZEROS-1];

function automatic flags_t reduce_flags_array (
    input flags_t flags_array [0:TOTAL_POLES_ZEROS-1]
);
    flags_t result;
    result = flags_array[0];

    for (int l = 1; l < TOTAL_POLES_ZEROS; l++) begin
        if (flags_array[l].valid !== result.valid ||
            flags_array[l].sof   !== result.sof   ||
            flags_array[l].eol   !== result.eol) begin
            result.valid = 1'b0;
        end
    end

    return result;
endfunction

function automatic logic reduce_logic (
    input logic logic_array [0:TOTAL_POLES_ZEROS-1]
);
    logic result;
    result = logic_array[0];

    for (int m = 1; m < TOTAL_POLES_ZEROS; m++) begin
        if (logic_array[m] != 1'b1) begin
            result = 1'b0;
        end
    end

    return result;
endfunction

function automatic flags_t merge_flags (
    input flags_t flags_1,
    input flags_t flags_2
);
    flags_t result;
    if (flags_1.valid && flags_2.valid &&
        flags_1.sof == flags_2.sof &&
        flags_1.eol == flags_2.eol) begin
        result = flags_1; 
    end else begin
        result = '{valid: 1'b0, sof: 1'b0, eol: 1'b0};
    end

    return result;
endfunction

flags_t flags_atan   [0:TOTAL_POLES_ZEROS-1];
flags_t flags_logmag [0:TOTAL_POLES_ZEROS-1];
flags_t flags_atan_reduced; 
flags_t flags_logmag_reduced;

flags_t flags_atan_reg [0:TOTAL_POLES_ZEROS-1];
logic [DATA_WIDTH-1:0] phases_reg [0:TOTAL_POLES_ZEROS-1];

logic atan_ready   [0:TOTAL_POLES_ZEROS-1];
logic log_mag_ready [0:TOTAL_POLES_ZEROS-1];
logic phase_ready, mag_ready;
logic log_mag_ready_reduced, atan_ready_reduced;

assign atan_ready_reduced = reduce_logic(atan_ready);
assign log_mag_ready_reduced = reduce_logic(log_mag_ready);
assign function_eval_ready = (atan_ready_reduced) ? log_mag_ready_reduced : 1'b0;

genvar k;
generate 
    for (k = 0; k < TOTAL_POLES_ZEROS; k++) begin
        atan_approx atan_approx_inst_i (
            .clk(clk), .resetn(periph_resetn),
            .ready_in(atan_ready[k]), .ready_out(phase_ready),
            .flags_in(coord_gen_flags), .flags_out(flags_atan_reg[k]),
            .x(diff_re[k]), .y(diff_im[k]),
            .angle(phases_reg[k])
        ); // 2 STAGE

        always_ff @(posedge clk) begin
            flags_atan[k] <= flags_atan_reg[k];
            phases[k] <= phases_reg[k];
        end // 1 STAGE

        log_mag_approx #(
            .DATA_WIDTH(DATA_WIDTH)
        ) log_mag_approx_inst_i (
            .clk(clk), .resetn(periph_resetn),
            .ready_in(log_mag_ready[k]), .ready_out(mag_ready),
            .flags_in(coord_gen_flags), .flags_out(flags_logmag[k]),
            .z_re(diff_re[k]), .z_im(diff_im[k]),
            .log2_mag(log_mags[k])
        ); // 3 STAGE
    end
endgenerate

// always_comb begin
//     $display("%d %d", log_mags[0], phases_reg[0]);
// end

assign flags_atan_reduced = reduce_flags_array(flags_atan);
assign flags_logmag_reduced = reduce_flags_array(flags_logmag);

/* verilator lint_off UNUSED */
logic [DATA_WIDTH-1:0] phase_sum; 
logic [DATA_WIDTH-1:0] log_mag_sum;
/* verilator lint_on UNUSED */

flags_t phase_flags, magnitude_flags;
logic colour_map_ready;

pz_difference #(
    .DATA_WIDTH(DATA_WIDTH),
    .TOTAL_POLES_ZEROS(TOTAL_POLES_ZEROS),
    .MAX_PZ(MAX_POLES_ZEROS)
) phase (
    .clk(clk),
    .resetn(periph_resetn),
    .in_data(phases),
    .num_zeros(num_zeros),
    .num_poles(num_poles),
    .flags_in(flags_atan_reduced), .in_ready(phase_ready),
    .diff_out(phase_sum),
    .flags_out(phase_flags), .out_ready(colour_map_ready)
); // log2(MAX_POLES_ZEROS) + 1 stages

pz_difference #(
    .DATA_WIDTH(DATA_WIDTH),
    .TOTAL_POLES_ZEROS(TOTAL_POLES_ZEROS),
    .MAX_PZ(MAX_POLES_ZEROS)
) magnitude (
    .clk(clk),
    .resetn(periph_resetn),
    .in_data(log_mags),
    .num_zeros(num_zeros),
    .num_poles(num_poles),
    .flags_in(flags_logmag_reduced), .in_ready(mag_ready),
    .diff_out(log_mag_sum),
    .flags_out(magnitude_flags), .out_ready(colour_map_ready)
); // log2(MAX_POLES_ZEROS) + 1 stages

// always_comb begin 
//     $display("%d %d", num_zeros, num_poles);
// end

// always_comb begin
//     $display("%b %b", log_mag_sum[15:6], phase_sum[15:4]);
// end

flags_t eval_flags, colour_flags;
assign eval_flags = merge_flags(phase_flags, magnitude_flags);

assign log_mag_sum = 'b0;

colour_map colour_map_inst(  
    .clk(clk),
    .resetn(periph_resetn),
    .hue(phase_sum[DATA_WIDTH-1 -: 11]),
    .log_mag(log_mag_sum[DATA_WIDTH-1 -: 9]),
    .flags_in(eval_flags), .in_ready(colour_map_ready),
    .r(r), .g(g), .b(b),
    .flags_out(colour_flags), .out_ready(pixel_packer_ready)
); // 1 STAGE

assign colour_flags_valid = colour_flags.valid;

packer pixel_packer(    
    .aclk(clk),
    .aresetn(periph_resetn),
    .r(r), .g(g), .b(b),
    .eol(colour_flags.eol), .in_stream_ready(pixel_packer_ready), .valid(colour_flags.valid), .sof(colour_flags.sof),
    .out_stream_tdata(out_stream_tdata), .out_stream_tkeep(out_stream_tkeep),
    .out_stream_tlast(out_stream_tlast), .out_stream_tready(out_stream_tready),
    .out_stream_tvalid(out_stream_tvalid), .out_stream_tuser(out_stream_tuser) 
);

 
endmodule
