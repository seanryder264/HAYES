module color(
    input aclk,
    input periph_resestn,
    
    input out_stream_ready,

    output [7:0] r,g,b

);



    wire signed [15:0] cordic_x = {{6{z_re[9]}}, z_re};
    wire signed [15:0] cordic_y = {{7{z_im[8]}}, z_im};

    fake_cordic cordic_sim (
        .x_in(cordic_x),
        .y_in(cordic_y),
        .mag_out(mag),
        .phase_out(phase)
    );

endmodule