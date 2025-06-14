module atan_approx (
    input wire clk,
    input wire resetn,
    input wire ready,
    input wire signed [15:0] x,
    input wire signed [15:0] y,
    output reg angle
);

    localparam [15:0] FULL_CIRCLE = 2**16-1;
    localparam [15:0] QUADRANT_SIZE = FULL_CIRCLE / 4;

    wire [15:0] abs_x;
    wire [15:0] abs_y;

    wire x_larger;

    wire [1:0] quad;
    wire [2:0] oct [0:1];

    wire [15:0] opp;
    wire [15:0] adj;

    wire [31:0] opp_32;
    wire [31:0] adj_32;

    /* verilator lint_off UNUSED */
    reg [31:0] ratio_32;
    /* verilator lint_on UNUSED */

    reg [15:0] atan_lut [0:191];
    initial $readmemh("atan_lut.mem", atan_lut);

    wire [7:0] ratio;
    wire [15:0] base_angle;

    always @* begin
        abs_x = (x > 0) ? x : -x;
        abs_y = (y > 0) ? y : -y;
        x_larger = (abs_x >= abs_y);
    end

    always @* begin
        if (x >= 0 && y >= 0)       quad = 2'd0;
        else if (x < 0 && y >= 0)   quad = 2'd1;
        else if (x < 0 && y < 0)    quad = 2'd2;
        else                        quad = 2'd3;

        case (quad)
            2'd0: oct = (x_larger) ? 3'd0 : 3'd1;
            2'd1: oct = (x_larger) ? 3'd3 : 3'd2;
            2'd2: oct = (x_larger) ? 3'd4 : 3'd5;
            2'd3: oct = (x_larger) ? 3'd7 : 3'd6;
        endcase
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
            oct[0] <= 3'd0;
        end else if (ready) begin
            ratio_32 <= opp_32 / adj_32;
            oct[1] <= oct[0];
        end else begin
            ratio_32 <= ratio_32;
            oct[1] <= oct[1];
        end
    end

    always @* begin
        ratio = ratio_32[DATA_WIDTH-1 -: 8];
        base_angle = atan_lut[ratio];
    end

    always @(posedge clk) begin
        if (!resetn) begin
            angle <= 'b0;
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
        end else begin
            angle <= angle;
        end
    end

endmodule
