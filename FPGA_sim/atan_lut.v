module atan_lut (
    input signed [15:0] x, 
    input signed [15:0] y,
    output reg [7:0]   angle
);

    wire [15:0] abs_x = (x > 0) ? x : -x;
    wire [15:0] abs_y = (y > 0) ? y : -y;

    wire [15:0] opp = (abs_x > abs_y) ? abs_y : abs_x;
    wire [15:0] adj = (abs_x > abs_y) ? abs_x : abs_y;

    wire [31:0] opp_32 = {opp, 16'b0};
    wire [31:0] adj_32 = {16'b0, adj};

   /* verilator lint_off UNUSED */
    wire [31:0] ratio_32 = opp_32 / adj_32;
    /* verilator lint_on UNUSED */
    wire [7:0] ratio = (ratio_32[31:16] != 0) ? 8'd255 : ratio_32[15:8];

    reg [7:0] atan_lut [0:255];
    initial $readmemb("atan_lut.mem", atan_lut);

    wire [7:0] base_angle = atan_lut[ratio];

    always @* begin
        angle = 0;
        if (x >= 0 && y >= 0) begin // Q1
            angle = (abs_x >= abs_y) ? base_angle : 8'd64 - base_angle ;
        end else if (x < 0 && y >= 0) begin // Q2
            angle = (abs_x >= abs_y) ? 8'd128 - base_angle : 8'd64 + base_angle;
        end else if (x < 0 && y < 0) begin // Q3
            angle = (abs_x >= abs_y) ? 8'd128 + base_angle : 8'd192 - base_angle;
        end else if (x >= 0 && y < 0) begin // Q4
            angle = (abs_x >= abs_y) ? 8'd255 - base_angle : 8'd192 + base_angle;
        end
    end

endmodule



