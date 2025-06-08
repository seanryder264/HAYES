module complex_sub(
    input clk,
    input resetn,
    input ready,

    input signed [15:0]    a_re,
    input signed [15:0]    a_im,
    input signed [15:0]    b_re,
    input signed [15:0]    b_im,
    output reg signed [15:0]   c_re,
    output reg signed [15:0]   c_im
);


always @(posedge clk) begin
        if (!resetn) begin
            c_re <= 16'b0;
            c_im <= 16'b0;
        end 
        else if (ready) begin
            c_re <= a_re - b_re;
            c_im <= a_im - b_im;
        end 
        else begin
            c_re <= c_re;
            c_im <= c_im;
        end
end

endmodule