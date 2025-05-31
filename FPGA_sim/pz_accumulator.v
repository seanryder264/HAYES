module pz_accumulator #(
    parameter REG_FILE_SIZE = 2
)(
    input wire clk,
    input wire [16*REG_FILE_SIZE-1:0] flat_pz,
    output reg [15:0] acc_pz
);

    integer i;
    reg signed [15:0] pz_i;

    always @(posedge clk) begin
        acc_pz <= 16'd0; 
        for (i = 0; i < REG_FILE_SIZE; i = i + 1) begin
            pz_i <= flat_pz[16*i +: 16];
            acc_pz <= acc_pz + pz_i;
        end
    end

endmodule
