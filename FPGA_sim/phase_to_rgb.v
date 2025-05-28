module phase_to_rgb (
    input  [8:0]        phase, 
    output reg [7:0]    red, 
    output reg [7:0]    green, 
    output reg [7:0]    blue
);
    /* verilator lint_off UNUSED */
    wire [8:0] scaled_temp_full;
    assign scaled_temp_full = (phase * 8'd255) / 9'd360;
    /* verilator lint_on UNUSED */

    wire [7:0] scaled_phase;
    assign scaled_phase = scaled_temp_full[7:0];


    always @* begin
        red   = 8'd0;
        green = 8'd0;
        blue  = 8'd0;

        if (scaled_phase < 8'd85) begin
            red   = 8'd255 - (scaled_phase * 3);
            green = scaled_phase * 3;
        end 
        else if (scaled_phase < 8'd170) begin
            green = 8'd255 - ((scaled_phase - 8'd85) * 3);
            blue  = (scaled_phase - 8'd85) * 3;
        end 
        else begin
            blue = 8'd255 - ((scaled_phase - 8'd170) * 3);
            red  = (scaled_phase - 8'd170) * 3;
        end
    end

endmodule

