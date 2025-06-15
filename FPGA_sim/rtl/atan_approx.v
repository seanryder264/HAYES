module atan_approx (
    input wire clk,
    input wire resetn,
    input wire ready,
    input wire signed [15:0] x,
    input wire signed [15:0] y,
    output reg [15:0] angle
);

    localparam FULL_CIRCLE = 2**16;
    localparam QUADRANT_SIZE = FULL_CIRCLE / 4;

    reg [15:0] abs_x;
    reg [15:0] abs_y;

    reg x_larger;

    reg [2:0] oct [0:1];

    reg [15:0] opp;
    reg [15:0] adj;

    reg [31:0] opp_32;
    reg [31:0] adj_32;

    /* verilator lint_off UNUSED */
    reg [31:0] ratio_32;
    /* verilator lint_on UNUSED */

    reg [15:0] atan_lut [0:255];
    initial $readmemb("atan_lut.mem", atan_lut);

    reg [7:0] ratio;
    reg [15:0] base_angle;

    always @* begin
        abs_x = (x > 0) ? x : -x;
        abs_y = (y > 0) ? y : -y;
        x_larger = (abs_x >= abs_y);
    end

    always @* begin
        if (x >= 0 && y >= 0)       oct[0] = (x_larger) ? 3'd0 : 3'd1;
        else if (x < 0 && y >= 0)   oct[0] = (x_larger) ? 3'd3 : 3'd2;
        else if (x < 0 && y < 0)    oct[0] = (x_larger) ? 3'd4 : 3'd5;
        else                        oct[0] = (x_larger) ? 3'd7 : 3'd6;
    end

    always @* begin
        opp = (x_larger) ? abs_y : abs_x;
        adj = (x_larger) ? abs_x : abs_y;

        opp_32 = {opp, 16'b0};
        adj_32 = {16'b0, adj};
    end

    always @(posedge clk) begin
        if (!resetn) begin
            ratio_32 <= 'b0;
            oct[1] <= 3'd0;
        end else if (ready) begin
            ratio_32 <= opp_32 / adj_32;
            oct[1] <= oct[0];
        end
    end

    always @* begin
        ratio = ratio_32[15:8];
        base_angle = atan_lut[ratio];
    end

    always @(posedge clk) begin
        if (!resetn) begin
            angle <= 16'b0;
        end else if (ready) begin
            case (oct[1])
                3'd0: angle <= base_angle;
                3'd1: angle <= QUADRANT_SIZE - base_angle;
                3'd2: angle <= QUADRANT_SIZE + base_angle;
                3'd3: angle <= 2*QUADRANT_SIZE - base_angle;
                3'd4: angle <= 2*QUADRANT_SIZE + base_angle;
                3'd5: angle <= 3*QUADRANT_SIZE - base_angle;
                3'd6: angle <= 3*QUADRANT_SIZE + base_angle;
                3'd7: angle <= FULL_CIRCLE - base_angle;
            endcase
        end
    end

endmodule
