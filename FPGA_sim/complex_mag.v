module complex_mag(
    input [9:0]     z_re,
    input [8:0]     z_im,
    output [9:0]    mag
);

    wire [20:0] sum_of_squares = (z_re * z_re) + (z_im * z_im);

    

endmodule
