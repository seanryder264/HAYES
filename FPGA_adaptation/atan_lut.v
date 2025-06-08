module atan_lut (
    input clk,
    input resetn,
    input ready,

    input signed [15:0] x, 
    input signed [15:0] y,
    output reg [7:0]   angle
);
    //Memory
    reg [7:0] atan_lut [0:255];
    initial $readmemb("atan_lut.mem", atan_lut);

    //Pipeline 1
    wire [15:0] abs_x = (x > 0) ? x : -x;
    wire [15:0] abs_y = (y > 0) ? y : -y;

    wire [15:0] opp = (abs_x > abs_y) ? abs_y : abs_x;
    wire [15:0] adj = (abs_x > abs_y) ? abs_x : abs_y;

    wire x_sign = x[15];
    wire y_sign = y[15];
    wire x_gt_y = (abs_x > abs_y);

    reg [15:0] opp_s2, adj_s2;
    reg        x_gt_y_s2;
    reg        x_sign_s2, y_sign_s2;

    //Pipeline 2
    wire [31:0] opp_32 = {opp_s2, 16'b0};
    wire [31:0] adj_32 = {16'b0, adj_s2};

   /* verilator lint_off UNUSED */
    wire [31:0] ratio_32 = opp_32 / adj_32;
    /* verilator lint_on UNUSED */
    wire [7:0] ratio = (ratio_32[31:16] != 0) ? 8'd255 : ratio_32[15:8];

    wire [7:0] base_angle = atan_lut[ratio];

    reg [7:0]  base_angle_s3;
    reg        x_gt_y_s3;
    reg        x_sign_s3, y_sign_s3;

    //Pipeline 3
    reg [7:0] final_angle;

    always @* begin
        final_angle = 0; 
        if (!x_sign_s3 && !y_sign_s3) begin // Q1: x>=0, y>=0
            final_angle = x_gt_y_s3 ? base_angle_s3 : 8'd64 - base_angle_s3;
        end else if (x_sign_s3 && !y_sign_s3) begin // Q2: x<0, y>=0
            final_angle = x_gt_y_s3 ? 8'd128 - base_angle_s3 : 8'd64 + base_angle_s3;
        end else if (x_sign_s3 && y_sign_s3) begin // Q3: x<0, y<0
            final_angle = x_gt_y_s3 ? 8'd128 + base_angle_s3 : 8'd192 - base_angle_s3;
        end else if (!x_sign_s3 && y_sign_s3) begin // Q4: x>=0, y<0
            final_angle = x_gt_y_s3 ? 8'd255 - base_angle_s3 : 8'd192 + base_angle_s3;
        end
    end

    //Control Logic
    always @(posedge clk) begin
        if (!resetn) begin
            opp_s2 <= 16'b0;
            adj_s2 <= 16'b0;
            x_gt_y_s2 <= 1'b0;
            x_sign_s2 <= 1'b0;
            y_sign_s2 <= 1'b0;
            base_angle_s3 <= 8'b0;
            x_gt_y_s3 <= 1'b0;
            x_sign_s3 <= 1'b0;
            y_sign_s3 <= 1'b0;
            angle <= 8'b0;
        end
        else if (ready) begin
            opp_s2 <= opp;
            adj_s2 <= adj;
            x_gt_y_s2 <= x_gt_y;
            x_sign_s2 <= x_sign;
            y_sign_s2 <= y_sign;

            base_angle_s3 <= base_angle;
            x_gt_y_s3 <= x_gt_y_s2;
            x_sign_s3 <= x_sign_s2;
            y_sign_s3 <= y_sign_s2;

            angle <= final_angle;
        end
    end

endmodule