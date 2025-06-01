module colour_map (
    /* verilator lint_off UNUSED */
    input  [15:0]       phase, 
    /* verilator lint_on UNUSED */
    output reg [7:0]    red, 
    output reg [7:0]    green, 
    output reg [7:0]    blue
);


    wire [7:0] phase_8 = phase[15:8];

    always @* begin
        red   = 8'd0;
        green = 8'd0;
        blue  = 8'd0;

        if (phase_8 < 8'd85) begin
            red   = 8'd255 - (phase_8 * 3);
            green = phase_8 * 3;
        end 
        else if (phase_8 < 8'd170) begin
            green = 8'd255 - ((phase_8 - 8'd85) * 3);
            blue  = (phase_8 - 8'd85) * 3;
        end 
        else begin
            blue = 8'd255 - ((phase_8 - 8'd170) * 3);
            red  = (phase_8 - 8'd170) * 3;
        end

        // $display("%d %d %d %d", phase_8, red, blue, green);
    end

endmodule

