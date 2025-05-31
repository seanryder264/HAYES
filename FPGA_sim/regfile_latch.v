module regfile_latch (
    input               clk,
    input               resetn,
    input               latch,
    input  [31:0]       wdata,
    output reg [31:0]   rdata
);

    always @(posedge clk) begin
        if (!resetn) begin
            rdata <= {32'b0};
        end else if (latch) begin
            rdata <= wdata;
        end
    end

endmodule
