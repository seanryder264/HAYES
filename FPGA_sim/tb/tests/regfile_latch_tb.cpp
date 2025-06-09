/*
 *  Verifies the results of the RegfileLatch, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class RegfileLatchTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        top->latch = 0;
        for (int i = 0; i < 32; i++){
             top->data_in[i] = 0;
        }
    }
};

TEST_F(RegfileLatchTestbench, RegfileLatch0WorksTest)
{
    top->resetn = 0;

    top->eval();

    for (int i = 0; i < 32; i++){
        EXPECT_EQ(top->data_out[i], 0);
    }
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