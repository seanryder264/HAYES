module coordinate_gen (
    input                       clk,
    input                       resetn,
    input                       ready,

    output reg signed [15:0]    x,
    output reg signed [15:0]    y, 
    output                      first,
    output                      lastx,
    output                      valid
);

    localparam signed [15:0] X_SIZE = 16'd640;
    localparam signed [15:0] Y_SIZE = 16'd480;

    wire signed [15:0] x_min = -X_SIZE / 2;
    wire signed [15:0] x_max = X_SIZE / 2 - 1;
    wire signed [15:0] y_min = 1 - Y_SIZE / 2;
    wire signed [15:0] y_max = Y_SIZE / 2;

    assign valid = 1'b1;
    //Changed to y_max look over this
    assign first  = (x == x_min) && (y == y_max);
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