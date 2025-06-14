module pixel_generator#(
    parameter AXI_LITE_ADDR_WIDTH = 8,
    parameter REG_FILE_SIZE = 8
)(
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
/* verilator lint_on UNUSED */
output          s_axi_lite_arready,
input           s_axi_lite_arvalid,

/* verilator lint_off UNUSED */
input [AXI_LITE_ADDR_WIDTH-1:0]     s_axi_lite_awaddr,
/* verilator lint_on UNUSED */
output          s_axi_lite_awready,
input           s_axi_lite_awvalid,

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

output [7:0] r, g, b,
output final_valid

);

localparam REG_FILE_AWIDTH = $clog2(REG_FILE_SIZE);

localparam AWAIT_WADD_AND_DATA = 3'b000;
localparam AWAIT_WDATA = 3'b001;
localparam AWAIT_WADD = 3'b010;
localparam AWAIT_WRITE = 3'b100;
localparam AWAIT_RESP = 3'b101;

localparam AWAIT_RADD = 2'b00;
localparam AWAIT_FETCH = 2'b01;
localparam AWAIT_READ = 2'b10;

localparam AXI_OK = 2'b00;
localparam AXI_ERR = 2'b10;

reg [31:0]                          regfile [REG_FILE_SIZE-1:0];
reg [REG_FILE_AWIDTH-1:0]           writeAddr, readAddr;
reg [31:0]                          readData, writeData;
reg [1:0]                           readState = AWAIT_RADD;
reg [2:0]                           writeState = AWAIT_WADD_AND_DATA;

//Read from the register file
always @(posedge s_axi_lite_aclk) begin
    
    readData <= regfile[readAddr];

    if (!axi_resetn) begin
    readState <= AWAIT_RADD;
    end

    else case (readState)

        AWAIT_RADD: begin
            if (s_axi_lite_arvalid) begin
                readAddr <= s_axi_lite_araddr[2+:REG_FILE_AWIDTH];
                readState <= AWAIT_FETCH;
            end
        end

        AWAIT_FETCH: begin
            readState <= AWAIT_READ;
        end

        AWAIT_READ: begin
            if (s_axi_lite_rready) begin
                readState <= AWAIT_RADD;
            end
        end

        default: begin
            readState <= AWAIT_RADD;
        end

    endcase
end

assign s_axi_lite_arready = (readState == AWAIT_RADD);
assign s_axi_lite_rresp = ({1'b0, readAddr} < REG_FILE_SIZE) ? AXI_OK : AXI_ERR;
assign s_axi_lite_rvalid = (readState == AWAIT_READ);
assign s_axi_lite_rdata = readData;

//Write to the register file, use a state machine to track address write, data write and response read events
always @(posedge s_axi_lite_aclk) begin

    if (!axi_resetn) begin
        writeState <= AWAIT_WADD_AND_DATA;
    end

    else case (writeState)

        AWAIT_WADD_AND_DATA: begin  //Idle, awaiting a write address or data
            case ({s_axi_lite_awvalid, s_axi_lite_wvalid})
                2'b10: begin
                    writeAddr <= s_axi_lite_awaddr[2+:REG_FILE_AWIDTH];
                    writeState <= AWAIT_WDATA;
                end
                2'b01: begin
                    writeData <= s_axi_lite_wdata;
                    writeState <= AWAIT_WADD;
                end
                2'b11: begin
                    writeData <= s_axi_lite_wdata;
                    writeAddr <= s_axi_lite_awaddr[2+:REG_FILE_AWIDTH];
                    writeState <= AWAIT_WRITE;
                end
                default: begin
                    writeState <= AWAIT_WADD_AND_DATA;
                end
            endcase        
        end

        AWAIT_WDATA: begin //Received address, waiting for data
            if (s_axi_lite_wvalid) begin
                writeData <= s_axi_lite_wdata;
                writeState <= AWAIT_WRITE;
            end
        end

        AWAIT_WADD: begin //Received data, waiting for address
            if (s_axi_lite_awvalid) begin
                writeAddr <= s_axi_lite_awaddr[2+:REG_FILE_AWIDTH];
                writeState <= AWAIT_WRITE;
            end
        end

        AWAIT_WRITE: begin //Perform the write
            regfile[writeAddr] <= writeData;
            writeState <= AWAIT_RESP;
        end

        AWAIT_RESP: begin //Wait to send response
            if (s_axi_lite_bready) begin
                writeState <= AWAIT_WADD_AND_DATA;
            end
        end

        default: begin
            writeState <= AWAIT_WADD_AND_DATA;
        end
    endcase
end

assign s_axi_lite_awready = (writeState == AWAIT_WADD_AND_DATA || writeState == AWAIT_WADD);
assign s_axi_lite_wready = (writeState == AWAIT_WADD_AND_DATA || writeState == AWAIT_WDATA);
assign s_axi_lite_bvalid = (writeState == AWAIT_RESP);
assign s_axi_lite_bresp = ({1'b0, writeAddr} < REG_FILE_SIZE) ? AXI_OK : AXI_ERR;


wire signed [15:0] z_re;
wire signed [15:0] z_im;
wire first, lastx, ready, valid_coord;

wire signed [31:0] reg_word [REG_FILE_SIZE-1:0];
wire signed [31:0] poles_and_zeros [REG_FILE_SIZE-1:0];
wire signed [15:0] w_re [REG_FILE_SIZE-1:0];
wire signed [15:0] w_im [REG_FILE_SIZE-1:0];
wire signed [15:0] diff_re [REG_FILE_SIZE-1:0];
wire signed [15:0] diff_im [REG_FILE_SIZE-1:0];
wire [15:0] phase [REG_FILE_SIZE-1:0];
wire [7:0] log_mag [REG_FILE_SIZE-1:0];
wire [(16 * REG_FILE_SIZE) - 1:0] phase_flat;
wire [(8 * REG_FILE_SIZE) - 1:0] log_mag_flat;

coordinate_gen coordinate_gen(  .clk(out_stream_aclk),
                                .resetn(periph_resetn),
                                .ready(ready),
                                .x(z_re), .y(z_im), 
                                .sof(first), .eol(lastx),
                                .valid(valid_coord));

genvar i;
generate 
    for (i = 0; i < 8; i = i + 1) begin
        assign reg_word[i] = regfile[i];


        assign poles_and_zeros[i] = reg_word[i];

        assign w_re[i] = poles_and_zeros[i][31:16];
        assign w_im[i] = poles_and_zeros[i][15:0];


        //Might want to reverse order of subtraction inside submodule
        complex_sub complex_sub_inst_i (
            .clk(out_stream_aclk),
            .resetn(periph_resetn),
            .ready(ready),
            .a_re(z_re), .a_im(z_im),
            .b_re(w_re[i]), .b_im(w_im[i]),
            .c_re(diff_re[i]), .c_im(diff_im[i])
        );

        // Look over this
        // atan_approx atan_approx_inst_i (
        //     .clk(out_stream_aclk),
        //     .resetn(periph_resetn),
        //     .ready(ready),
        //     .x(diff_re[i]), .y(diff_im[i]),
        //     .angle(phase[i])
        // );

        // wire [17:0] phase_temp;

        // cordic_0 cordic(
        //     .aclk(out_stream_aclk),
        //     .aclken(ready),
        //     .s_axis_cartesian_tdata({diff_re[i], diff_im[i]}),
        //     .m_axis_dout_tdata(phase_temp)
        // );
        
        // assign phase[i] = phase_temp[15:0];

        assign phase[i] = 0;

        wire [7:0] mag_temp;
        //Added mag_temp wire in case it decides to optimise a register away
        //Look over this
        log_mag_calc mag_calc_inst_i (
            .clk(out_stream_aclk),
            .resetn(periph_resetn),
            .ready(ready),
            .x(diff_re[i]), .y(diff_im[i]),
            .log_mag(mag_temp)
        );

        assign log_mag[i] = mag_temp;

        assign phase_flat[i*16 +: 16] = phase[i];
        assign log_mag_flat[i*8 +: 8] = log_mag[i];
    end
endgenerate

wire [31:0] no_z = 32'd1;
wire [31:0] no_p = 32'd0;

//Might need to increase bits here to deal with accumulator sum
wire [15:0] acc_phase;
wire [7:0] acc_log_mag;

//Look over this
//Needs to changed if you change REG_FILE_SIZE
pz_accumulator #(
    .REG_FILE_SIZE(REG_FILE_SIZE),
    .DATA_SIZE(16)
) phase_accumulator (
    .clk(out_stream_aclk),
    .resetn(periph_resetn),
    .ready(ready),
    .no_z(no_z), .no_p(no_p),
    .flat_pz(phase_flat),
    .acc_pz(acc_phase)
);

pz_accumulator #(
    .REG_FILE_SIZE(REG_FILE_SIZE),
    .DATA_SIZE(8)
) log_mag_accumulator (
    .clk(out_stream_aclk),
    .resetn(periph_resetn),
    .ready(ready),
    .no_z(no_z), .no_p(no_p),
    .flat_pz(log_mag_flat),
    .acc_pz(acc_log_mag)
);

colour_app colour_app_inst( 
    .clk(out_stream_aclk),
    .resetn(periph_resetn),
    .ready(ready), 
    .phase(acc_phase),
    .log_mag(acc_log_mag),
    .red(r), .green(g), .blue(b) 
);

localparam PIPE_LATENCY = 29; // Total latency of the data path

reg [PIPE_LATENCY:0] valid_pipe;
reg [PIPE_LATENCY:0] first_pipe;
reg [PIPE_LATENCY:0] lastx_pipe;

always @(posedge out_stream_aclk) begin
    if (!periph_resetn) begin
        valid_pipe <= 0;
        first_pipe <= 0;
        lastx_pipe <= 0;
    end 
    else if (ready) begin
        valid_pipe <= {valid_pipe[PIPE_LATENCY-1:0], valid_coord};
        first_pipe <= {first_pipe[PIPE_LATENCY-1:0], first};
        lastx_pipe <= {lastx_pipe[PIPE_LATENCY-1:0], lastx};
    end
end

// The final, delayed control signals emerge at the end of the pipeline
assign final_valid = valid_pipe[PIPE_LATENCY];
wire final_first = first_pipe[PIPE_LATENCY];
wire final_lastx = lastx_pipe[PIPE_LATENCY];




//Do the wiring for packer: Valid needs to be redone
packer pixel_packer(    .aclk(out_stream_aclk),
                        .aresetn(periph_resetn),
                        .r(r), .g(g), .b(b),
                        .eol(final_lastx), .in_stream_ready(ready), .valid(final_valid), .sof(final_first),
                        .out_stream_tdata(out_stream_tdata), .out_stream_tkeep(out_stream_tkeep),
                        .out_stream_tlast(out_stream_tlast), .out_stream_tready(out_stream_tready),
                        .out_stream_tvalid(out_stream_tvalid), .out_stream_tuser(out_stream_tuser) );

 
endmodule
