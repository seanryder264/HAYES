module latch_poles_zeros (
    input wire             clk,
    input wire             reset,
    input wire             frame_done,
    input wire [31:0]      zero_in [3:0],   // 4 complex zeroes: {re, im}
    input wire [31:0]      pole_in [3:0],   // 4 complex poles:  {re, im}
    output reg [31:0]      zero [3:0],      // Latched zeroes
    output reg [31:0]      pole [3:0]       // Latched poles
);

    integer i;

    always @(posedge clk) begin
        if (reset) begin
            for (i = 0; i < 4; i = i + 1) begin
                zero[i] <= 32'd0;
                pole[i] <= 32'd0;
            end
        end else if (frame_done) begin
            for (i = 0; i < 4; i = i + 1) begin
                zero[i] <= zero_in[i];
                pole[i] <= pole_in[i];
            end
        end
    end

endmodule
