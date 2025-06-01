module frame_reg (
    input wire             clk,
    input wire             reset,
    input wire             frame_done,
    input wire [31:0]      zero_in_0, zero_in_1, zero_in_2, zero_in_3,
    input wire [31:0]      pole_in_0, pole_in_1, pole_in_2, pole_in_3,
    output [31:0]      zero_0, zero_1, zero_2, zero_3,
    output [31:0]      pole_0, pole_1, pole_2, pole_3
);

    always @(posedge clk) begin
        if (reset) begin
            zero_0 <= 32'd0; zero_1 <= 32'd0; zero_2 <= 32'd0; zero_3 <= 32'd0;
            pole_0 <= 32'd0; pole_1 <= 32'd0; pole_2 <= 32'd0; pole_3 <= 32'd0;
        end else if (frame_done) begin
            zero_0 <= zero_in_0; zero_1 <= zero_in_1; zero_2 <= zero_in_2; zero_3 <= zero_in_3;
            pole_0 <= pole_in_0; pole_1 <= pole_in_1; pole_2 <= pole_in_2; pole_3 <= pole_in_3;
        end
    end

endmodule
