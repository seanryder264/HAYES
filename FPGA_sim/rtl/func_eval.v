module func_eval #(
    parameter POLE_COUNT = 4,
    parameter ZERO_COUNT = 4
    parameter DATA_WIDTH = 16
)(
    input wire [DATA_WIDTH:0] x_in,
    input wire [DATA_WIDTH:0] y_in,
    output wire [DATA_WIDTH:0] numerator_re,
    output wire [DATA_WIDTH:0] numerator_im,
    output wire [DATA_WIDTH:0] denominator_re,
    output wire [DATA_WIDTH:0] denominator_im, 
);

// Registers that hold the values of 4 poles and 4 zeroes
    reg [31:0] poles [0:3];
    reg [31:0] zeros [0:3];

// Add blocks first
// Then multiply
// Then output the numerator and denominator seperately


adder0 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[0][DATA_WIDTH:0]),
    .b_im(poles[0][DATA_WIDTH:0]),
    .add_re(adder_out0_re),
    .add_im(adder_out0_im)
);
wire [DATA_WIDTH-1:0] adder_out0_re
wire [DATA_WIDTH-1:0] adder_out0_im

adder1 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[1][DATA_WIDTH:0]),
    .b_im(poles[1][DATA_WIDTH:0]),
    .add_re(adder_out1_re),
    .add_im(adder_out1_im)
);
wire [DATA_WIDTH-1:0] adder_out1_re
wire [DATA_WIDTH-1:0] adder_out1_im

adder2 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[2][DATA_WIDTH:0]),
    .b_im(poles[2][DATA_WIDTH:0]),
    .add_re(adder_out2_re),
    .add_im(adder_out2_im)
);
wire [DATA_WIDTH-1:0] adder_out2_re
wire [DATA_WIDTH-1:0] adder_out2_im

adder3 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[3][DATA_WIDTH:0]),
    .b_im(poles[3][DATA_WIDTH:0]),
    .add_re(adder_out3_re),
    .add_im(adder_out3_im)
);
wire [DATA_WIDTH-1:0] adder_out3_re
wire [DATA_WIDTH-1:0] adder_out3_im

adder4 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[4][DATA_WIDTH:0]),
    .b_im(poles[4][DATA_WIDTH:0]),
    .add_re(adder_out4_re),
    .add_im(adder_out4_im)
);
wire [DATA_WIDTH-1:0] adder_out4_re
wire [DATA_WIDTH-1:0] adder_out4_im

adder5 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[5][DATA_WIDTH:0]),
    .b_im(poles[5][DATA_WIDTH:0]),
    .add_re(adder_out5_re),
    .add_im(adder_out5_im)
);
wire [DATA_WIDTH-1:0] adder_out5_re
wire [DATA_WIDTH-1:0] adder_out5_im

adder6 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[6][DATA_WIDTH:0]),
    .b_im(poles[6][DATA_WIDTH:0]),
    .add_re(adder_out6_re),
    .add_im(adder_out6_im)
);
wire [DATA_WIDTH-1:0] adder_out6_re
wire [DATA_WIDTH-1:0] adder_out6_im

adder7 add_complex(
    .a_re(x_in),
    .a_im(y_in),
    .b_re(poles[7][DATA_WIDTH:0]),
    .b_im(poles[7][DATA_WIDTH:0]),
    .add_re(adder_out7_re),
    .add_im(adder_out7_im)
);
wire [DATA_WIDTH-1:0] adder_out7_re
wire [DATA_WIDTH-1:0] adder_out7_im


// Adder 0 and 1 in
mul_z0 mul_complex(
    .a_re(adder_out0_re),
    .a_im(adder_out0_im),
    .b_re(adder_out1_re),
    .b_im(adder_out1_im),
    .mul_re(z0_re),
    .mul_im(z0_im)
);
wire z0_re;
wire z0_im;

// Adder 2 and 3 in
mul_z1 mul_complex(
    .a_re(),
    .a_im(),
    .b_re(),
    .b_im(),
    .mul_re(z1_re),
    .mul_im(z1_im)
);
wire z1_re;
wire z1_im;

mul_z2 mul_complex(
    .a_re(z0_re),
    .a_im(z0_im),
    .b_re(z1_re),
    .b_im(z1_im),
    .mul_re(numerator_re),
    .mul_im(numerator_im)
);


mul_p0 mul_complex(
    .a_re(a_re),
    .a_im(a_im),
    .b_re(b_re),
    .b_im(b_im),
    .mul_re(numerator_re),
    .mul_im(numerator_im)
);

mul_p1 mul_complex(
    .a_re(a_re),
    .a_im(a_im),
    .b_re(b_re),
    .b_im(b_im),
    .mul_re(numerator_re),
    .mul_im(numerator_im)
);

mul_p2 mul_complex(
    .a_re(a_re),
    .a_im(a_im),
    .b_re(b_re),
    .b_im(b_im),
    .mul_re(numerator_re),
    .mul_im(numerator_im)
);




    
endmodule