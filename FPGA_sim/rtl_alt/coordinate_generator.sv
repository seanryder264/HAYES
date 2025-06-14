module coordinate_generator #(
    parameter int DATA_WIDTH = 16,
    parameter logic [DATA_WIDTH-1:0] X_SIZE = 2048,
    parameter logic [DATA_WIDTH-1:0] Y_SIZE = 2048
)(
    input clk,
    input resetn,
    input ready,
 
    output logic signed [DATA_WIDTH-1:0]    x, 
    output logic signed [DATA_WIDTH-1:0]    y,
    output                                  done,
    output                                  valid,
    output                                  sof,
    output                                  eol   
);

    localparam logic signed [DATA_WIDTH-1:0] X_MIN = - X_SIZE / 2;
    localparam logic signed [DATA_WIDTH-1:0] X_MAX = X_SIZE / 2 - 1;
    localparam logic signed [DATA_WIDTH-1:0] Y_MIN = 1 - Y_SIZE / 2;
    localparam logic signed [DATA_WIDTH-1:0] Y_MAX = Y_SIZE / 2;

    typedef enum logic [2:0] {WAIT, START, DATA} state_t;

    state_t next_state = WAIT;
    state_t current_state = WAIT;

    logic signed [DATA_WIDTH-1:0] next_x = X_MAX;
    logic signed [DATA_WIDTH-1:0] next_y = Y_MIN;
    logic next_valid = 0;
    logic next_sof = 0;
    logic next_eol = 0;
    logic next_done = 0;

    always_comb begin
        case (current_state)
            WAIT    : if (ready) next_state = (done) ? START : DATA;
            START   : next_state = DATA;
            DATA    : if (done || !ready) next_state = WAIT;
            default : next_state = current_state;
        endcase
    end

    always_ff @(posedge clk) begin
        if (!resetn) begin
            current_state <= WAIT;
        end else begin
            current_state <= next_state;
        end
    end

    always_comb begin

        case (current_state)
            WAIT    : begin
                next_x = x;
                next_y = y;
                next_valid = 0;
                next_sof = 0;
                next_eol = (x == X_MAX);
                next_done = (x == X_MAX && y == Y_MIN);
            end
            START   : begin
                next_x = X_MIN;
                next_y = Y_MAX;
                next_valid = 1;
                next_sof = 1;
                next_eol = 0;
                next_done = 0;
            end
            DATA    : begin
                next_x = (eol) ? X_MIN : x + 1;
                next_y = (eol) ? y - 1 : y;
                next_valid = 1;
                next_sof = 0;
                next_eol = (x == X_MAX);
                next_done = (x == X_MAX && y == Y_MIN);
            end
            default :;
        endcase
    end

    always_ff @(posedge clk) begin
        if (!resetn) begin
            x <= X_MAX;
            y <= Y_MIN;
            valid <= 0;
            sof <= 0;
            eol <= 0;
            done <= 1;
        end else begin
            x <= next_x;
            y <= next_y;
            valid <= next_valid;
            sof <= next_sof;
            eol <= next_eol;
            done <= next_done;
        end
    end

endmodule
