/*
 *  Verifies the results of the AtanApprox, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class AtanApproxTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        top->x = 0;
        top->y = 0;
    }
};

TEST_F(AtanApproxTestbench, AtanApprox0WorksTest)
{
    top->resetn = 0;

    top->eval();

    EXPECT_EQ(top->angle, 0);
}

TEST_F(AtanApproxTestbench, AtanApprox1WorksTest)
{
    top->resetn = 1;
    top->ready_out = 1;
    top->x = 640;
    top->y = 480;

    runSimulation(1); // prime stage 1
    runSimulation(1); // let stage 2 compute

    printf("Final angle = %04x\n", top->angle);
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