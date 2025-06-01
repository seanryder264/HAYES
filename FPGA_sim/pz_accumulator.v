module pz_accumulator #(
    parameter REG_FILE_SIZE = 2,
    parameter DATA_SIZE = 16
)(
    input wire clk,
    input wire [DATA_SIZE*REG_FILE_SIZE-1:0] flat_pz,
    input wire [31:0] no_z,
    input wire [31:0] no_p,
    output reg signed [DATA_SIZE - 1:0] acc_pz
);

    integer i;
    reg signed [DATA_SIZE - 1:0] acc_z_temp, acc_p_temp;
    reg signed [DATA_SIZE - 1:0] z_i, p_i;

    always @* begin
        acc_z_temp = 0;
        acc_p_temp = 0;

        for (i = 0; i < no_z; i = i + 1) begin
            z_i = flat_pz[DATA_SIZE*i +: DATA_SIZE];
            acc_z_temp = acc_z_temp + z_i;
        end

        for (i = no_z; i < no_z + no_p; i = i + 1) begin
            p_i = flat_pz[DATA_SIZE*i +: DATA_SIZE];
            acc_p_temp = acc_p_temp + p_i;
        end
    end

    always @(posedge clk) begin
        acc_pz <= acc_z_temp - acc_p_temp;
    end

endmodule

