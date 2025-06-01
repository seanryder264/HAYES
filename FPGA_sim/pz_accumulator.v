module pz_accumulator #(
    parameter REG_FILE_SIZE = 2
)(
    input wire clk,
    input wire [16*REG_FILE_SIZE-1:0] flat_pz,
    input wire [31:0] no_z,
    input wire [31:0] no_p,
    output reg signed [15:0] acc_pz
);

    integer i;
    reg signed [15:0] acc_z_temp, acc_p_temp;
    reg signed [15:0] z_i, p_i;

    always @* begin
        acc_z_temp = 16'sd0;
        acc_p_temp = 16'sd0;

        for (i = 0; i < no_z; i = i + 1) begin
            z_i = flat_pz[16*i +: 16];
            acc_z_temp = acc_z_temp + z_i;
        end

        for (i = no_z; i < no_z + no_p; i = i + 1) begin
            p_i = flat_pz[16*i +: 16];
            acc_p_temp = acc_p_temp + p_i;
        end
    end

    always @(posedge clk) begin
        acc_pz <= acc_z_temp - acc_p_temp;
    end

endmodule

