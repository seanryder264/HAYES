module coord_gen (
    input                   clk,
    input                   resetn,
    input                   ready,

    output reg signed [9:0] x,
    output reg signed [8:0] y, 
    output                  first,
    output                  lastx,
    output                  valid
);

    localparam signed [9:0] X_SIZE = 640;
    localparam signed [8:0] Y_SIZE = 480;

    wire signed [9:0] x_min = -X_SIZE / 2;          // -320
    wire signed [9:0] x_max = X_SIZE / 2 - 1;       // +319
    wire signed [8:0] y_min = 1 - Y_SIZE / 2;       // -239
    wire signed [8:0] y_max = Y_SIZE / 2;           // +240

    assign valid = 1'b1;
    assign first  = (x == x_min) && (y == y_min);
    assign lastx = (x == x_max);

    always @(posedge clk) begin
        if (!resetn) begin
            x <= x_min;
            y <= y_max;
        end else if (ready && valid) begin
            if (x == x_max) begin
                x <= x_min;
                y <= (y == y_min) ? y_max : y - 1;
            end else begin
                x <= x + 1;
                y <= y;
            end
        end else begin
            x <= x;
            y <= y;
        end
    end


endmodule
