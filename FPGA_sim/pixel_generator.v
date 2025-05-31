module pixel_generator(
input           out_stream_aclk,
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

input           s_axi_lite_bready,
output [1:0]    s_axi_lite_bresp,
output          s_axi_lite_bvalid,

output [31:0]   s_axi_lite_rdata,
input           s_axi_lite_rready,
output [1:0]    s_axi_lite_rresp,
output          s_axi_lite_rvalid,

input  [31:0]   s_axi_lite_wdata,
output          s_axi_lite_wready,
input           s_axi_lite_wvalid,

output [7:0] r, g, b

);

parameter  REG_FILE_SIZE = 2;

reg [31:0]  regfile [REG_FILE_SIZE-1:0];

data_mem #( 
    .REG_FILE_SIZE(REG_FILE_SIZE)
) data_mem_inst (  .aclk(s_axi_lite_aclk), 
    .resetn(axi_resetn),
    .regfile_flat(regfile_flat)
    .araddr(s_axi_lite_araddr),
    .arready(s_axi_lite_arready),
    .arvalid(s_axi_lite_arvalid),
    .awaddr(s_axi_lite_awaddr),
    .awready(s_axi_lite_awready),
    .arvalid(s_axi_lite_awvalid),
    .bready(s_axi_lite_bready),
    .bresp(s_axi_lite_bresp),
    .bvalid(s_axi_lite_bvalid),
    .rdata(s_axi_lite_rdata),
    .rready(s_axi_lite_rready),
    .rresp(s_axi_lite_rresp),
    .rvalid(s_axi_lite_rvalid),
    .wdata(s_axi_lite_wdata),
    .wready(s_axi_lite_wready),
    .wvalid(s_axi_lite_wvalid));

wire signed [15:0] z_re;
wire signed [15:0] z_im;
wire first, lastx, ready, valid_int;

coordinate_gen coordinate_gen(  .clk(out_stream_aclk),
                                .resetn(periph_resetn),
                                .ready(ready),
                                .x(z_re), .y(z_im), 
                                .first(first), .lastx(lastx),
                                .valid(valid_int));

reg [31:0] poles_and_zeros [REG_FILE_SIZE-1:0];
wire [16:0] w_re [REG_FILE_SIZE-1:0];
wire [16:0] w_im [REG_FILE_SIZE-1:0];
wire [16:0] diff_re [REG_FILE_SIZE-1:0];
wire [16:0] diff_im [REG_FILE_SIZE-1:0];
wire [16:0] phase [REG_FILE_SIZE-1:0];

genvar i;
generate 
    for (i = 0; i < REG_FILE_SIZE; i++) begin : per_pole
        wire [31:0] reg_word = regfile_flat[i*32 +: 32];

        regfile_latch regfile_latch_inst_i (
            .clk(out_stream_aclk),
            .resetn(periph_resetn),
            .latch(first),
            .wdata(reg_word),
            .rdata(poles_and_zeros[i])
        );

        assign w_re[i] = poles_and_zeros[i][31:16];
        assign w_im[i] = poles_and_zeros[i][15:0];

        complex_sub complex_sub_inst_i (
            .a_re(z_re), .a_im(z_im),
            .b_re(w_re[i]), .b_im(w_im[i]),
            .c_re(diff_re[i]), .c_im(diff_im[i])
        );

        atan_lut atan_lut_inst_i (
            .x(diff_re[i]), .y(diff_im[i]),
            .angle(phase[i])
        );

        wire []
    end
endgenerate

phase_eval phase_eval();

phase_to_rgb phase_to_rgb(  .phase(phase),
                            .red(r), .green(g), .blue(b) );

packer pixel_packer(    .aclk(out_stream_aclk),
                        .aresetn(periph_resetn),
                        .r(r), .g(g), .b(b),
                        .eol(lastx), .in_stream_ready(ready), .valid(valid_int), .sof(first),
                        .out_stream_tdata(out_stream_tdata), .out_stream_tkeep(out_stream_tkeep),
                        .out_stream_tlast(out_stream_tlast), .out_stream_tready(out_stream_tready),
                        .out_stream_tvalid(out_stream_tvalid), .out_stream_tuser(out_stream_tuser) );

 
endmodule
