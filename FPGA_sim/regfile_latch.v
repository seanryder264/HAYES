module regfile_latch #(
    parameter REG_FILE_SIZE = 8
)(
    input                           clk,
    input                           resetn,
    input                           latch,
    input  [32*REG_FILE_SIZE-1:0]   wdata_flat,
    output reg [32*REG_FILE_SIZE-1:0] rdata_flat
);

    integer i;

    always @(posedge clk) begin
        if (!resetn) begin
            rdata_flat <= {32*REG_FILE_SIZE{1'b0}};
        end else if (latch) begin
            for (i = 0; i < REG_FILE_SIZE; i = i + 1) begin
                rdata_flat[i*32 +: 32] <= wdata_flat[i*32 +: 32];
            end
        end
    end

endmodule
