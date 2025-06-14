module regfile_latch #(
    parameter int DATA_WIDTH = 16,
    parameter logic [5:0] REG_FILE_SIZE = 32
)(
    input  logic                       clk,
    input  logic                       resetn,
    input  logic                       latch,
    input  logic                       ready,
    input  logic [DATA_WIDTH-1:0]      data_in     [0:REG_FILE_SIZE-1],

    output logic [DATA_WIDTH-1:0]      data_out    [0:REG_FILE_SIZE-1],
    output logic                       latch_ready
);

    typedef enum logic {LATCH, DATA} state_t;

    state_t current_state, next_state;

    logic [DATA_WIDTH-1:0] latch_data [0:REG_FILE_SIZE-1];

    // FSM transition logic
    always_comb begin
        next_state = current_state;
        case (current_state)
            DATA  : if (latch) next_state = LATCH;
            LATCH : next_state = DATA;
        endcase
    end

    // FSM state update
    always_ff @(posedge clk) begin
        if (!resetn)
            current_state <= DATA;
        else
            current_state <= next_state;
    end

    // Latch the input into internal registers on LATCH state
    always_ff @(posedge clk) begin
        if (current_state == LATCH) begin
            for (int i = 0; i < REG_FILE_SIZE; i++) begin
                latch_data[i] <= data_in[i];
            end
        end
    end

    // Output latch contents
    always_ff @(posedge clk) begin
        for (int i = 0; i < REG_FILE_SIZE; i++) begin
            data_out[i] <= latch_data[i];
        end
    end

    // latch_ready signal logic
    always_ff @(posedge clk) begin
        if (!resetn)
            latch_ready <= 0;
        else if (current_state == LATCH)
            latch_ready <= 0;
        else
            latch_ready <= ready; // only propagate ready signal in DATA state
    end

endmodule
