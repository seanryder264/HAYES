module log_mag_calc (
    input clk,
    input resetn,
    input ready,

    input signed [15:0] x,
    input signed [15:0] y,
    output reg [7:0] log_mag
);

    reg [31:0] x_sqr;
    reg [31:0] y_sqr;

    reg [32:0] mag_sqr;

    integer i;
    reg [5:0] msb_pos;
    reg found;

    /* verilator lint_off UNUSED */
    reg [32:0] norm_mag_full;
    /* verilator lint_on UNUSED */

    reg [7:0] log_lut [0:255];
    initial $readmemb("log_lut.mem", log_lut);

    always @(posedge clk) begin
        if (!resetn) begin
            x_sqr <= 32'b0;
            y_sqr <= 32'b0;
        end else if (ready) begin
            x_sqr <= x * x;
            y_sqr <= y * y;
        end else begin
            x_sqr <= x_sqr;
            y_sqr <= y_sqr;
        end
    end // STAGE 1

    always @(posedge clk) begin
        if (!resetn) begin
            mag_sqr <= 'b0;
        end else if (ready) begin
            mag_sqr <= x_sqr + y_sqr;
        end else begin
            mag_sqr <= mag_sqr;
        end
    end // STAGE 2

    always @* begin
        msb_pos = 6'b0;
        found = 0;
        for (i = 32; i >= 0; i--) begin
            if (mag_sqr[i] && !found) begin
                msb_pos = i[5:0];
                found = 1; 
            end
        end

        norm_mag_full = (mag_sqr << 16) >> msb_pos;
    end

    always @(posedge clk) begin
        if (!resetn) begin
            log_mag <= 'b0;
        end else if (ready) begin
            log_mag <= log_lut[norm_mag_full[15:8]];
        end else begin
            log_mag <= log_mag;
        end
    end // STAGE 3
       
endmodule
