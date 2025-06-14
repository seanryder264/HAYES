import types_pkg::flags_t;

module atan_approx #(
    parameter int DATA_WIDTH = 16
)(
    input logic                         clk,
    input logic                         resetn,
    output logic                        ready_in,
    input logic                         ready_out,
    input flags_t                       flags_in,
    output flags_t                      flags_out,
    input logic signed [DATA_WIDTH-1:0] x,  y,
    output logic [DATA_WIDTH-1:0]       angle
);
    localparam logic [DATA_WIDTH-1:0] FULL_CIRCLE = 6*256;
    localparam logic [DATA_WIDTH-1:0] QUADRANT_SIZE = FULL_CIRCLE / 4;

    typedef enum logic [1:0] {
        Q1, Q2, Q3, Q4
    } quadrant_t;

    quadrant_t quad;

    typedef enum logic [2:0] {
        O1, O2, O3, O4, O5, O6, O7, O8
    } octant_t;

    localparam int NUM_STAGES = 2;

    typedef struct packed {
        octant_t oct;
        flags_t flags;
    } meta_t;

    meta_t meta [0:NUM_STAGES-1];
    assign meta[0].flags = flags_in;
    assign ready_in = ready_out;

    logic [DATA_WIDTH-1:0] abs_x;
    logic [DATA_WIDTH-1:0] abs_y;

    logic [DATA_WIDTH-1:0] opp;
    logic [DATA_WIDTH-1:0] adj;

    logic [(2*DATA_WIDTH)-1:0] opp_32;
    logic [(2*DATA_WIDTH)-1:0] adj_32;

    logic x_larger;

    always_comb begin
        abs_x = (x > 0) ? x : -x;
        abs_y = (y > 0) ? y : -y;
        x_larger = (abs_x >= abs_y);
    end

    always_comb begin
        abs_x = (x > 0) ? x : -x;
        abs_y = (y > 0) ? y : -y;

        if (x >= 0 && y >= 0)       quad = Q1;
        else if (x < 0 && y >= 0)   quad = Q2;
        else if (x < 0 && y < 0)    quad = Q3;
        else                        quad = Q4;

        if (x_larger) begin
            case (quad)
                Q1: meta[0].oct = O1;
                Q2: meta[0].oct = O4;
                Q3: meta[0].oct = O5;
                Q4: meta[0].oct = O8;
            endcase
        end else begin
            case (quad)
                Q1: meta[0].oct = O2;
                Q2: meta[0].oct = O3;
                Q3: meta[0].oct = O6;
                Q4: meta[0].oct = O7;
            endcase
        end
    end

    always_comb begin
        if (x_larger) begin
            opp = abs_y;
            adj = abs_x;
        end else begin
            opp = abs_x;
            adj = abs_y;
        end

        opp_32 = {opp, 16'b0};
        adj_32 = {16'b0, adj};
    end

    /* verilator lint_off UNUSED */
    logic [(2*DATA_WIDTH)-1:0] ratio_32;
    /* verilator lint_on UNUSED */

    always_ff @(posedge clk) begin
        if (!resetn) begin
            ratio_32 <= 'b0;
            meta[1].flags.valid <= 0;
        end else if (ready_out) begin
            ratio_32 <= opp_32 / adj_32;
            meta[1] <= meta[0];
        end else begin
            ratio_32 <= ratio_32;
            meta[1] <= meta[1];
        end
    end

    logic [DATA_WIDTH-1:0] atan_lut [0:191];
    initial $readmemh("atan_lut.mem", atan_lut);

    logic [7:0] ratio;
    logic [DATA_WIDTH-1:0] base_angle;

    always_comb begin
        ratio = ratio_32[DATA_WIDTH-1 -: 8];
        base_angle = atan_lut[ratio];
    end

    always_ff @(posedge clk) begin
        if (!resetn) begin
            angle <= 'b0;
            flags_out.valid <= 0; 
        end else if (ready_out) begin

            case (meta[1].oct)
                O1: angle <= base_angle;
                O2: angle <= QUADRANT_SIZE - base_angle;
                O3: angle <= QUADRANT_SIZE + base_angle;
                O4: angle <= 2*QUADRANT_SIZE - base_angle;
                O5: angle <= 2*QUADRANT_SIZE + base_angle;
                O6: angle <= 3*QUADRANT_SIZE - base_angle;
                O7: angle <= 3*QUADRANT_SIZE + base_angle;
                O8: angle <= FULL_CIRCLE - base_angle;
                default: angle <= 0;
            endcase

            flags_out <= meta[1].flags;
        end else begin
            angle <= angle;
            flags_out <= flags_out;
        end
    end

endmodule
