module moduleName #(
    parameter COORD_WIDTH = 32,
    parameter X_WIDTH = 640,
    parameter YWIDTH = 480
) (
    input wire clk,
    input wire reset,
    output logic signed [COORD_WIDTH-1:0] x_out,
    output logic signed [COORD_WIDTH-1:0] y_out,
) (
    // Generate normalised coordinates

    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            x_out <= 0;
            y_out <= 0;
        end else begin
            // Example logic to generate coordinates
            // This should be replaced with actual coordinate generation logic
            x_out <= $signed($random) % X_WIDTH; // Random x coordinate within width
            y_out <= $signed($random) % YWIDTH;  // Random y coordinate within height
        end
    end

);
    
endmodule