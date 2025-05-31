module complex_sub (
    input  signed [15:0] x,
    input  signed [15:0] y,

    input  [31:0] zero [0:3],  // 4 zeroes: {re, im}
    input  [31:0] pole [0:3],  // 4 poles:  {re, im}

    output signed [15:0] zero_diff_re [0:3],
    output signed [15:0] zero_diff_im [0:3],
    output signed [15:0] pole_diff_re [0:3],
    output signed [15:0] pole_diff_im [0:3]
);

    integer i;
    always @* begin
        for (i = 0; i < 4; i = i + 1) begin
            // Subtract each zero: z - zero[i]
            zero_diff_re[i] = x - $signed(zero[i][31:16]);
            zero_diff_im[i] = y - $signed(zero[i][15:0]);

            // Subtract each pole: z - pole[i]
            pole_diff_re[i] = x - $signed(pole[i][31:16]);
            pole_diff_im[i] = y - $signed(pole[i][15:0]);
        end
    end

endmodule
