/*
 *  Verifies the results of the PZDifference, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class PZDifferenceTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        for (int i = 0; i < 16; i++){
            top->in_data[i] = 0;
        }
        top->num_zeros = 0;
        top->num_poles = 0;
        top->out_ready = 1;
    }
};

TEST_F(PZDifferenceTestbench, PZDifference0WorksTest)
{
    top->resetn = 0;

    top->eval();

    EXPECT_EQ(top->diff_out, 0);
}

TEST_F(PZDifferenceTestbench, PZDifference1WorksTest)
{
    runSimulation(1);

    for (int i = 0; i < 8; i++){
        top->in_data[i] = 1024;
    }
    for (int i = 8; i < 16; i++){
        top->in_data[i] = 512;
    }

    top->num_zeros = 8;
    top->num_poles = 8;

    runSimulation(1);

    for(int i = 0; i < 5; i++){ runSimulation(1); }
    
    EXPECT_EQ(top->diff_out, 4096);

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