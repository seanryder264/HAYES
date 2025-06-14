module polynomial_eval #(
    parameter int DATA_WIDTH = 16,  
)(
    input                                   clk,
    input  logic signed [DATA_WIDTH-1:0]    x,
    input  logic signed [DATA_WIDTH-1:0]    a [0:5],
    output logic signed [DATA_WIDTH-1:0]    fx
);

    logic [DATA_WIDTH-1:0] mul1_1, mul1_2, mul1_3, x2_1;
    logic [DATA_WIDTH-1:0] sum2_1, sum2_2, sum2_3, x2_2, x4_2;
    logic [DATA_WIDTH-1:0] term3_1, term3_2, term3_3;
    logic [DATA_WIDTH-1:0] sum4_1, term4_3;
    logic [DATA_WIDTH-1:0] sum5_2;

    always_ff @(posedge clk) begin
        mul1_1 <= a[1] * x;
        mul1_2 <= a[3] * x;
        mul1_3 <= a[5] * x;
        x2_1 <= x * x;

        sum2_1 <= a[0] + mul1_1;
        sum2_2 <= a[2] + mul1_2;
        sum2_3 <= a[4] + mul1_3;
        x2_2 <= x2_1;
        x4_2 <= x2_1 * x2_1;

        term3_1 <= sum2_1;
        term3_2 <= sum2_2 * x2_2;
        term3_3 <= sum2_3 * x4_2;

        sum4_1 <= term3_1 + term3_2;
        term4_3 <= term3_3;

        sum5_2 <= sum4_1 + term4_4;
    end

    assign fx = sum5_2;

endmodule

