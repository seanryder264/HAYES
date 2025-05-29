module square_sum_unsigned (
    input  wire signed [15:0] in_re,
    input  wire signed [15:0] in_im,
    output wire [31:0] sum_sq
);
=
    wire [31:0] re_sq = in_re * in_re;
    wire [31:0] im_sq = in_im * in_im;
    assign sum_sq = re_sq + im_sq;

endmodule
