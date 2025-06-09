module pz_accumulator #(
    parameter REG_FILE_SIZE = 8,
    parameter DATA_SIZE = 8
)(
    input wire clk,
    input resetn,
    input ready,

    input wire [DATA_SIZE*REG_FILE_SIZE-1:0] flat_pz,
    /* verilator lint_off UNUSED */
    input wire [31:0] no_z, // Number of 'zero' terms to process
    input wire [31:0] no_p, // Number of 'pole' terms to process
    /* verilator lint_on UNUSED */
    output reg [DATA_SIZE - 1:0] acc_pz
);

    //Pipeline 1
    // Use lower 4 bits for requested counts
    wire [3:0] requested_nz = no_z[3:0];
    wire [3:0] requested_np = no_p[3:0];

    wire [DATA_SIZE-1:0] z_terms [0:REG_FILE_SIZE-1];
    wire [DATA_SIZE-1:0] p_terms [0:REG_FILE_SIZE-1];

    genvar i;
    generate
        for (i = 0; i < REG_FILE_SIZE; i = i + 1) begin : term_selector
            assign z_terms[i] = (i < requested_nz) ? flat_pz[DATA_SIZE*i +: DATA_SIZE] : 0;
            assign p_terms[i] = ((i >= requested_nz) && (i < (requested_nz + requested_np))) ? flat_pz[DATA_SIZE*i +: DATA_SIZE] : 0;
        end
    endgenerate

    wire [DATA_SIZE:0] z_sum1_s1 [0:REG_FILE_SIZE/2-1];
    wire [DATA_SIZE:0] p_sum1_s1 [0:REG_FILE_SIZE/2-1];

    generate
        for (i = 0; i < REG_FILE_SIZE/2; i = i + 1) begin : adder_stage1
            assign z_sum1_s1[i] = z_terms[2*i] + z_terms[2*i+1];
            assign p_sum1_s1[i] = p_terms[2*i] + p_terms[2*i+1];
        end
    endgenerate

    reg [DATA_SIZE:0] z_sum1_s2 [0:REG_FILE_SIZE/2-1];
    reg [DATA_SIZE:0] p_sum1_s2 [0:REG_FILE_SIZE/2-1];

    //Pipeline 2
    wire [DATA_SIZE+1:0] z_sum2_s2 [0:REG_FILE_SIZE/4-1];
    wire [DATA_SIZE+1:0] p_sum2_s2 [0:REG_FILE_SIZE/4-1];

    generate
        for (i = 0; i < REG_FILE_SIZE/4; i = i + 1) begin : adder_stage2
            assign z_sum2_s2[i] = z_sum1_s2[2*i] + z_sum1_s2[2*i+1];
            assign p_sum2_s2[i] = p_sum1_s2[2*i] + p_sum1_s2[2*i+1];
        end
    endgenerate

    reg [DATA_SIZE+1:0] z_sum2_s3 [0:REG_FILE_SIZE/4-1];
    reg [DATA_SIZE+1:0] p_sum2_s3 [0:REG_FILE_SIZE/4-1];

    //Pipeline 3
    wire [DATA_SIZE+2:0] acc_z_final = z_sum2_s3[0] + z_sum2_s3[1];
    wire [DATA_SIZE+2:0] acc_p_final = p_sum2_s3[0] + p_sum2_s3[1];

    /* verilator lint_off UNUSED */
    wire [DATA_SIZE+2:0] final_result = acc_z_final - acc_p_final;
     /* verilator lint_on UNUSED */

    //Control Logic
    integer j;
    
    always @(posedge clk) begin
        if (!resetn) begin
            acc_pz    <= 0;

            for (j = 0; j < REG_FILE_SIZE/2; j = j + 1) begin
                z_sum1_s2[j] <= 0;
                p_sum1_s2[j] <= 0;
            end

            for (j = 0; j < REG_FILE_SIZE/4; j = j + 1) begin
                z_sum2_s3[j] <= 0;
                p_sum2_s3[j] <= 0;
            end
        end 
        else if (ready) begin
            for (j = 0; j < REG_FILE_SIZE/2; j = j + 1) begin
                z_sum1_s2[j] <= z_sum1_s1[j];
                p_sum1_s2[j] <= p_sum1_s1[j];
            end

            for (j = 0; j < REG_FILE_SIZE/4; j = j + 1) begin
                z_sum2_s3[j] <= z_sum2_s2[j];
                p_sum2_s3[j] <= p_sum2_s2[j];
            end
            
            acc_pz    <= final_result[7:0]; // Implicit truncation to DATA_SIZE
        end
    end

endmodule
