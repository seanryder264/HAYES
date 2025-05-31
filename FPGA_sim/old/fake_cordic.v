module fake_cordic (
    input  logic signed [15:0] x_in,
    input  logic signed [15:0] y_in,
    output logic signed [15:0] mag_out,
    output logic signed [15:0] phase_out
);
    real x_real, y_real;
    real mag_real, phase_real;

    always_comb begin
        // Convert fixed-point to real
        x_real = x_in / 4096.0;  // Q4.12
        y_real = y_in / 4096.0;

        // Compute magnitude and phase
        mag_real   = sqrt(x_real * x_real + y_real * y_real);
        phase_real = $atan2(y_real, x_real);

        // Convert real to fixed-point outputs
        mag_out   = mag_real * 4096.0;                  // back to Q4.12
        phase_out = phase_real * (32768.0 / 3.1415926); // [-π, π] → [-32768, 32767]
    end
endmodule
