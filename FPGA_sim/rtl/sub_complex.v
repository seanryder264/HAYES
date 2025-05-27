module complex_adder (
    input  wire [31:0] a_re,
    input  wire [31:0] a_im,
    input  wire [31:0] b_re,
    input  wire [31:0] b_im,
    output wire [31:0] sum_re,
    output wire [31:0] sum_im
);

    assign sum_real = a_real - b_real;
    assign sum_imag = a_imag - b_imag;

endmodule
