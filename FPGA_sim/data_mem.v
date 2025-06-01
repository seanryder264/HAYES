module data_mem #(
    parameter  REG_FILE_SIZE = 8,
    parameter  AXI_LITE_ADDR_WIDTH = 8
)(
    input   aclk,
    input   resetn,

    output reg [(32 * REG_FILE_SIZE) - 1:0]  regfile_flat,

    /* verilator lint_off UNUSED */
    // read address 
    input [AXI_LITE_ADDR_WIDTH-1:0]     araddr,
    output                              arready,
    input                               arvalid,

    // write address 
    input [AXI_LITE_ADDR_WIDTH-1:0]     awaddr,
    output                              awready,
    input                               awvalid,
    /* verilator lint_on UNUSED */

    // write response
    input           bready,
    output [1:0]    bresp,
    output          bvalid,

    // read data & response
    output [31:0]   rdata,
    input           rready,
    output [1:0]    rresp,
    output          rvalid,

    // write data
    input  [31:0]   wdata,
    output          wready,
    input           wvalid
);

    localparam int REG_ADDR_WIDTH = $bits(REG_FILE_SIZE);
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

    reg [REG_FILE_AWIDTH-1:0]           writeAddr, readAddr;
    reg [31:0]                          readData, writeData;
    reg [1:0]                           readState = AWAIT_RADD;
    reg [2:0]                           writeState = AWAIT_WADD_AND_DATA;
    reg [31:0]                          regfile [REG_FILE_SIZE-1:0];

    integer i;

    always @* begin
        for (i = 0; i < REG_FILE_SIZE; i = i + 1) begin
            assign regfile_flat[i*32 +: 32] = regfile[i];
        end
    end

    initial begin
        regfile[0]     = {16'd100, 16'd100}; // First entry
        regfile[1]    = {-16'd100, -16'd100}; // Second entry 
    end


    //Read from the register file
    always @(posedge aclk) begin
        
        readData <= regfile[readAddr];

        if (!resetn) begin
        readState <= AWAIT_RADD;
        end

        else case (readState)

            AWAIT_RADD: begin
                if (arvalid) begin
                    readAddr <= araddr[2+:REG_FILE_AWIDTH];
                    readState <= AWAIT_FETCH;
                end
            end

            AWAIT_FETCH: begin
                readState <= AWAIT_READ;
            end

            AWAIT_READ: begin
                if (rready) begin
                    readState <= AWAIT_RADD;
                end
            end

            default: begin
                readState <= AWAIT_RADD;
            end

        endcase
    end

    assign arready = (readState == AWAIT_RADD);
    assign rresp = ({{(REG_ADDR_WIDTH-3){1'b0}}, readAddr} < REG_FILE_SIZE) ? AXI_OK : AXI_ERR;
    assign rvalid = (readState == AWAIT_READ);
    assign rdata = readData;

    //Write to the register file, use a state machine to track address write, data write and response read events
    always @(posedge aclk) begin

        if (!resetn) begin
            writeState <= AWAIT_WADD_AND_DATA;
        end

        else case (writeState)

            AWAIT_WADD_AND_DATA: begin  //Idle, awaiting a write address or data
                case ({awvalid, wvalid})
                    2'b10: begin
                        writeAddr <= awaddr[2+:REG_FILE_AWIDTH];
                        writeState <= AWAIT_WDATA;
                    end
                    2'b01: begin
                        writeData <= wdata;
                        writeState <= AWAIT_WADD;
                    end
                    2'b11: begin
                        writeData <= wdata;
                        writeAddr <= awaddr[2+:REG_FILE_AWIDTH];
                        writeState <= AWAIT_WRITE;
                    end
                    default: begin
                        writeState <= AWAIT_WADD_AND_DATA;
                    end
                endcase        
            end

            AWAIT_WDATA: begin //Received address, waiting for data
                if (wvalid) begin
                    writeData <= wdata;
                    writeState <= AWAIT_WRITE;
                end
            end

            AWAIT_WADD: begin //Received data, waiting for address
                if (awvalid) begin
                    writeAddr <= awaddr[2+:REG_FILE_AWIDTH];
                    writeState <= AWAIT_WRITE;
                end
            end

            AWAIT_WRITE: begin //Perform the write
                regfile[writeAddr] <= writeData;
                writeState <= AWAIT_RESP;
            end

            AWAIT_RESP: begin //Wait to send response
                if (bready) begin
                    writeState <= AWAIT_WADD_AND_DATA;
                end
            end

            default: begin
                writeState <= AWAIT_WADD_AND_DATA;
            end
        endcase
    end

    assign awready = (writeState == AWAIT_WADD_AND_DATA || writeState == AWAIT_WADD);
    assign wready = (writeState == AWAIT_WADD_AND_DATA || writeState == AWAIT_WDATA);
    assign bvalid = (writeState == AWAIT_RESP);
    assign bresp = ({{(REG_ADDR_WIDTH-3){1'b0}}, writeAddr} < REG_FILE_SIZE) ? AXI_OK : AXI_ERR;

endmodule
