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

wire [DATA_WIDTH-1:0] adder_out0_re;
wire [DATA_WIDTH-1:0] adder_out0_im;
wire [DATA_WIDTH-1:0] adder_out1_re;
wire [DATA_WIDTH-1:0] adder_out1_im;
wire [DATA_WIDTH-1:0] adder_out2_re;
wire [DATA_WIDTH-1:0] adder_out2_im;
wire [DATA_WIDTH-1:0] adder_out3_re;
wire [DATA_WIDTH-1:0] adder_out3_im;

wire [DATA_WIDTH-1:0] adder_out4_re;
wire [DATA_WIDTH-1:0] adder_out4_im;
wire [DATA_WIDTH-1:0] adder_out5_re;
wire [DATA_WIDTH-1:0] adder_out5_im;
wire [DATA_WIDTH-1:0] adder_out6_re;
wire [DATA_WIDTH-1:0] adder_out6_im;
wire [DATA_WIDTH-1:0] adder_out7_re;
wire [DATA_WIDTH-1:0] adder_out7_im;

wire [DATA_WIDTH-1:0] mul_out0_re;  
wire [DATA_WIDTH-1:0] mul_out0_im;  
wire [DATA_WIDTH-1:0] mul_out1_re;  
wire [DATA_WIDTH-1:0] mul_out1_im;  
wire [DATA_WIDTH-1:0] numerator_re;  
wire [DATA_WIDTH-1:0] numerator_im;  
wire [DATA_WIDTH-1:0] mul_out3_re;  
wire [DATA_WIDTH-1:0] mul_out3_im;  
wire [DATA_WIDTH-1:0] mul_out4_re;  
wire [DATA_WIDTH-1:0] mul_out4_im;  
wire [DATA_WIDTH-1:0] denominator_re;  
wire [DATA_WIDTH-1:0] denominator_im;  

assign adder_out0_re = x_in + poles[0][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out0_im = y_in + poles[0][DATA_WIDTH:0];
assign adder_out1_re = x_in + poles[1][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out1_im = y_in + poles[1][DATA_WIDTH:0];
assign adder_out2_re = x_in + poles[2][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out2_im = y_in + poles[2][DATA_WIDTH:0];
assign adder_out3_re = x_in + poles[3][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out3_im = y_in + poles[3][DATA_WIDTH:0];

assign mul_out0_re = adder_out0_re * adder_out1_re - adder_out0_im * adder_out1_im;
assign mul_out0_im = adder_out0_re * adder_out1_im + adder_out0_im * adder_out1_re;

assign mul_out1_re = adder_out2_re * adder_out3_re - adder_out2_im * adder_out3_im;
assign mul_out1_im = adder_out2_re * adder_out3_im + adder_out2_im * adder_out3_re;

assign numerator_re = mul_out0_re * mul_out1_re - mul_out0_im * mul_out1_im;
assign numerator_im = mul_out0_re * mul_out1_im + mul_out0_im * mul_out1_re;

assign adder_out4_re = x_in + zeros[0][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out4_im = y_in + zeros[0][DATA_WIDTH:0];
assign adder_out5_re = x_in + zeros[1][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out5_im = y_in + zeros[1][DATA_WIDTH:0];
assign adder_out6_re = x_in + zeros[2][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out6_im = y_in + zeros[2][DATA_WIDTH:0];
assign adder_out7_re = x_in + zeros[3][2*DATA_WIDTH:DATA_WIDTH];
assign adder_out7_im = y_in + zeros[3][DATA_WIDTH:0];

assign mul_out3_re = adder_out4_re * adder_out5_re - adder_out4_im * adder_out5_im;
assign mul_out3_im = adder_out4_re * adder_out5_im + adder_out4_im * adder_out5_re;

assign mul_out4_re = adder_out6_re * adder_out7_re - adder_out6_im * adder_out7_im;
assign mul_out4_im = adder_out6_re * adder_out7_im + adder_out6_im * adder_out7_re;

assign denominator_re = mul_out3_re * mul_out4_re - mul_out3_im * mul_out4_im;
assign denominator_im = mul_out3_re * mul_out4_im + mul_out3_im * mul_out4_re;

    
endmodule