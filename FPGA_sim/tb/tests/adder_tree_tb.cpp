/*
 *  Verifies the results of the AdderTree, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class AdderTreeTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        top->in_data[0] = 0;
        top->in_data[1] = 0;
        top->in_data[2] = 0;
        top->in_data[3] = 0;
        top->in_data[4] = 0;
        top->in_data[5] = 0;
        top->in_data[6] = 0;
        top->in_data[7] = 0;
        top->num_valid_inputs = 0;
        top->eol_in = 0;
        top->sof_in = 0;
        top->valid_in = 0;
        top->out_ready = 0;
    }
};

TEST_F(AdderTreeTestbench, AdderTree0WorksTest)
{
    top->resetn = 0;

    top->eval();

    EXPECT_EQ(top->sum_out, 0);
    EXPECT_EQ(top->valid_out, 0);
}

TEST_F(AdderTreeTestbench, AdderTree1WorksTest)
{
    runSimulation(1);

    top->in_data[0] = 1024;
    top->in_data[1] = 1024;
    top->in_data[2] = 1024;
    top->in_data[3] = 1024;
    top->in_data[4] = 1024;
    top->in_data[5] = 1024;
    top->in_data[6] = 1024;
    top->in_data[7] = 1024;
    top->num_valid_inputs = 8;
    top->valid_in = 1;

    runSimulation(1);

    top->in_data[0] = 0;
    top->in_data[1] = 0;
    top->in_data[2] = 0;
    top->in_data[3] = 0;
    top->in_data[4] = 0;
    top->in_data[5] = 0;
    top->in_data[6] = 0;
    top->in_data[7] = 0;
    top->num_valid_inputs = 0;
    top->valid_in = 0;

    for(int i = 0; i < 3; i++){ runSimulation(1); }
    
    EXPECT_EQ(top->sum_out, 8192);
    EXPECT_EQ(top->valid_out, 1);

    runSimulation(1);
    runSimulation(1);
}


int main(int argc, char **argv)
{
    top = new Vdut;
    tfp = new VerilatedVcdC;

    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("waveform.vcd");

    testing::InitGoogleTest(&argc, argv);
    auto res = RUN_ALL_TESTS();

    top->final();
    tfp->close();

    delete top;
    delete tfp;

    return res;
}