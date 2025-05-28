module atan_lut (
    input  signed [9:0] x, 
    input  signed [8:0] y,
    output reg [8:0]    angle
);

    reg [5:0] atan_lut [0:1023];
    initial $readmemb("atan_lut.mem", atan_lut);

    wire [9:0] abs_x = (x > 0) ? x : -x;
    wire [9:0] abs_y = (y > 0) ? {1'b0, y} : {1'b0, -y};

    wire [9:0] opp = (abs_x > abs_y) ? abs_y : abs_x;
    wire [9:0] adj = (abs_x > abs_y) ? abs_x : abs_y;

    wire [9:0] ratio = opp / adj;

    wire [8:0] base_angle = {3'd0, atan_lut[ratio]};

    always @* begin
        angle = 0;
        if (x >= 0 && y >= 0) begin // Q1
            angle = (abs_x >= abs_y) ? base_angle : 9'd90 - base_angle ;
        end else if (x < 0 && y >= 0) begin // Q2
            angle = (abs_x >= abs_y) ? 9'd180 - base_angle : 9'd90 + base_angle;
        end else if (x < 0 && y < 0) begin // Q3
            angle = (abs_x >= abs_y) ? 9'd180 + base_angle : 9'd270 - base_angle;
        end else if (x >= 0 && y < 0) begin // Q4
            angle = (abs_x >= abs_y) ? 9'd360 - base_angle : 9'd270 + base_angle;
        end
    end

endmodule



