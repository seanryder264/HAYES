module coordinate_gen (
    input                       clk,
    input                       resetn,
    input                       ready,

    output reg signed [15:0]    x,
    output reg signed [15:0]    y, 
    output                      sof,
    output                      eol,
    output                      valid
);

    localparam signed [15:0] X_SIZE = 16'd640;
    localparam signed [15:0] Y_SIZE = 16'd480;

    localparam signed [15:0] X_MIN = -X_SIZE / 2;
    localparam signed [15:0] X_MAX = X_SIZE / 2 - 1;
    localparam signed [15:0] Y_MIN = 1 - Y_SIZE / 2;
    localparam signed [15:0] Y_MAX = Y_SIZE / 2;

    reg signed [15:0] next_x = X_MIN;
    reg signed [15:0] next_y = Y_MAX;

    always @* begin
        sof = (x == X_MIN) && (y == Y_MAX);
        eol = (x == X_MAX);

        next_x =    (!eol)          ? x + 1 : X_MIN;
        next_y =    (!eol)          ? y :
                    (y == Y_MIN)    ? Y_MAX : y - 1;
    end

    always @(posedge clk) begin
        if (!resetn) begin
            x <= X_MIN - 1;
            y <= Y_MAX;
            valid <= 0;
        end else if (ready) begin
            x <= next_x;
            y <= next_y;
            valid <= 1;
        end else begin
            x <= x;
            y <= y;
            valid <= 0;
        end
    end


endmodule
