module log_mag_calc (
    input clk,
    input resetn,
    input ready,

    input signed [15:0] x,
    input signed [15:0] y,
    output reg [7:0] log_mag
);

    //Pipeline 1
    wire [31:0] sqr_x = x * x;
    wire [31:0] sqr_y = y * y;

    reg [31:0] sqr_x_s2;
    reg [31:0] sqr_y_s2;

    //Pipeline 2
    wire [32:0] mag_sqr = sqr_x_s2 + sqr_y_s2;

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

    reg [32:0] mag_sqr_s3;
    reg [5:0]  log2_int_s3;

    //Pipeline 3
    //Need to look over this, what if it's seven or less
    
    reg [7:0] frac;
    always @(*) begin
        if (log2_int_s3 < 8) begin
            frac = mag_sqr_s3[7:0]; 
        end else begin
            frac = mag_sqr_s3[(log2_int_s3 - 1) -: 8];
        end
    end

    //Control Logic
    always @(posedge clk) begin
        if (!resetn) begin
            sqr_x_s2 <= 32'b0;
            sqr_y_s2 <= 32'b0;
            
            mag_sqr_s3 <= 33'b0;
            log2_int_s3 <= 6'b0;

            log_mag <= 8'b0;
        end
        else if (ready) begin
            sqr_x_s2 <= sqr_x;
            sqr_y_s2 <= sqr_y;
            
            mag_sqr_s3 <= mag_sqr;
            log2_int_s3 <= log2_int;

            log_mag <= frac;
        end
    end



    //assign log_mag = frac;          
endmodule
