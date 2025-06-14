import types_pkg::flags_t;


module adder_tree #(
    parameter int DATA_WIDTH = 16,
    parameter int NUM_INPUTS = 8 
)(
    input                                   clk,
    input                                   resetn,

    input logic [DATA_WIDTH-1:0]            in_data [0:NUM_INPUTS-1],
    input logic [$clog2(NUM_INPUTS):0]    num_valid_inputs,
    input flags_t                           flags_in,
    output                                  in_ready,

    output logic [DATA_WIDTH-1:0]           sum_out,
    output flags_t                          flags_out,
    input                                   out_ready
);

    localparam int MAX_LEVELS = $clog2(NUM_INPUTS); 

    logic [DATA_WIDTH-1:0] data_sum [0:MAX_LEVELS][0:NUM_INPUTS-1];

    genvar i;
    generate
        for (i = 0; i < NUM_INPUTS; i++) begin : DATA_MASK
            assign data_sum[0][i] = (i < num_valid_inputs)  ? in_data[i] : '0;
        end
    endgenerate

    genvar level, j;
    generate 
        for (level = 0; level < MAX_LEVELS; level++) begin : STAGE
            for (j = 0; j < NUM_INPUTS >> (level+1); j++) begin : ADD_STAGE
                always_ff @(posedge clk) begin
                    if(!resetn) begin
                        data_sum[level+1][j] <= '0;
                    end else begin
                        data_sum[level+1][j] <= data_sum[level][2*j] + data_sum[level][2*j+1];
                    end
                end
            end
        end
    endgenerate

    assign sum_out = data_sum[MAX_LEVELS][0];

    // Control Signal Pipelines

    flags_t flags_shift_reg [0:MAX_LEVELS-1];
    genvar k;
    generate
        for (k = 0; k < MAX_LEVELS; k++) begin
            always_ff @(posedge clk) begin
                if (!resetn) begin
                    flags_shift_reg[k] <= '0;
                end else if (out_ready) begin
                    flags_shift_reg[k+1] <= flags_shift_reg[k];
                    flags_shift_reg[0] <= flags_in;
                end else begin
                    flags_shift_reg <= flags_shift_reg;
                end
            end
        end
    endgenerate
    assign flags_out = flags_shift_reg[MAX_LEVELS-1];
    assign in_ready = out_ready;

endmodule
