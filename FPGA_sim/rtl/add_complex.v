module complex_adder #(
    parameter DATA_WIDTH = 16
) (
    input  wire [DATA_WIDTH:0] a_re,
    input  wire [DATA_WIDTH:0] a_im,
    input  wire [DATA_WIDTH:0] b_re,
    input  wire [DATA_WIDTH:0] b_im,
    output wire [DATA_WIDTH:0] sum_re,
    output wire [DATA_WIDTH:0] sum_im
);

    assign sum_real = a_real + b_real;
    assign sum_imag = a_imag + b_imag;

endmodule
