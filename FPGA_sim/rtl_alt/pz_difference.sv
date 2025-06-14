import types_pkg::flags_t;

module pz_difference #(
    parameter int DATA_WIDTH        = 16,
    parameter int TOTAL_POLES_ZEROS = 16,  // Must be even
    parameter int MAX_PZ            = 8
)(
    input  logic                        clk,
    input  logic                        resetn,

    input  logic [DATA_WIDTH-1:0]       in_data [0:TOTAL_POLES_ZEROS-1],
    input  logic [$clog2(MAX_PZ):0]   num_zeros,
    input  logic [$clog2(MAX_PZ):0]   num_poles,

    input  flags_t                      flags_in,
    output logic                        in_ready,

    output logic [DATA_WIDTH-1:0]       diff_out,
    output flags_t                      flags_out,
    input logic                         out_ready
);
    logic [DATA_WIDTH-1:0] zeros_data [MAX_PZ];
    logic [DATA_WIDTH-1:0] poles_data [MAX_PZ];

    genvar i;
    generate
        for (i = 0; i < MAX_PZ; i++) begin
            assign zeros_data[i] = in_data[i];
            assign poles_data[i] = in_data[i + MAX_PZ];
        end
    endgenerate

    logic [DATA_WIDTH-1:0] sum_zeros, sum_poles;
    flags_t flags_zeros, flags_poles;
    logic ready_zeros, ready_poles;

    assign in_ready = ready_zeros;

    adder_tree #(
        .DATA_WIDTH(DATA_WIDTH),
        .NUM_INPUTS(MAX_PZ)
    ) sum_zeros_tree (
        .clk(clk), .resetn(resetn),
        .in_data(zeros_data),
        .num_valid_inputs(num_zeros),
        .flags_in(flags_in),
        .in_ready(ready_zeros),
        .sum_out(sum_zeros),
        .flags_out(flags_zeros),
        .out_ready(ready_poles)
    ); // $log2(MAX_PZ) stages

    adder_tree #(
        .DATA_WIDTH(DATA_WIDTH),
        .NUM_INPUTS(MAX_PZ)
    ) sum_poles_tree (
        .clk(clk), .resetn(resetn),
        .in_data(poles_data),
        .num_valid_inputs(num_poles),
        .flags_in(flags_in),
        .in_ready(ready_poles),
        .sum_out(sum_poles),
        .flags_out(flags_poles),
        .out_ready(out_ready)
    ); // $log2(MAX_PZ) stages

    function automatic flags_t merge_flags (
        input flags_t flags_1,
        input flags_t flags_2
    );
        flags_t result;
        if (flags_1.valid && flags_2.valid &&
            flags_1.sof == flags_2.sof &&
            flags_1.eol == flags_2.eol) begin
            result = flags_1; 
        end else begin
            result = '{valid: 1'b0, sof: 1'b0, eol: 1'b0};
        end

        return result;
    endfunction

    always_ff @(posedge clk) begin
        if (!resetn) begin
            diff_out <= '0;
            flags_out <= '0;
        end else if (out_ready) begin
            diff_out <= sum_zeros - sum_poles;
            flags_out <= merge_flags(flags_zeros, flags_poles);
        end else begin
            diff_out <= diff_out;
            flags_out <= flags_out;
        end
    end // STAGE 1

endmodule
