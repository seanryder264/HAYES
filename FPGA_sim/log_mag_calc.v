module log_mag_calc (
    input signed [15:0] x,
    input signed [15:0] y,
    output [7:0] log_mag
);

    wire [31:0] sqr_x = x * x;
    wire [31:0] sqr_y = y * y;
    wire [32:0] mag_sqr = sqr_x + sqr_y;

    function [5:0] log2_floor;
        input [32:0] val;
        integer i;
        reg found;
        begin
            log2_floor = 0;
            found = 0;
            for (i = 32; i >= 0; i = i - 1) begin
                if (!found && val[i]) begin
                    log2_floor = i[5:0];
                    found = 1;
                end
            end
        end
    endfunction


    wire [5:0] log2_int = log2_floor(mag_sqr);
    wire [7:0] frac = mag_sqr[(log2_int-1) -: 8];  // 10-bit fractional interpolation

    assign log_mag = frac;          
endmodule
