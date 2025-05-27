module coord_gen (
    input                   out_stream_aclk,
    input                   periph_resetn,
    input                   ready,

    output reg signed [9:0] z_re,
    output reg signed [8:0] z_im, 
    output                  first_pixel,
    output                  last_re,
    output                  valid
);

    localparam signed [9:0] X_SIZE = 640;
    localparam signed [8:0] Y_SIZE = 480;

    wire signed [9:0] re_min = -X_SIZE / 2;          // -320
    wire signed [9:0] re_max = X_SIZE / 2 - 1;       // +319
    wire signed [8:0] im_min = 1 - Y_SIZE / 2;       // -239
    wire signed [8:0] im_max = Y_SIZE / 2;           // +240

    wire last_im = (z_im == im_min);
    wire last_pixel = last_re && last_im;

    assign valid = 1'b1;
    assign first_pixel = (z_re == re_min) && (z_im == im_max);
    assign last_re = (z_re == re_max);

    always @(posedge out_stream_aclk) begin
        if (!periph_resetn) begin
            z_re <= re_min;
            z_im <= im_max;
        end else if (ready & valid) begin
            if (last_pixel) begin
                z_re <= re_min;
                z_im <= im_max;
            end else if (last_re) begin
                z_re <= re_min;
                z_im <= z_im - 1;
            end else begin
                z_re <= z_re + 1;
            end
        end
    end

endmodule
