/*
 *  Verifies the results of the LogMagApprox, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class LogMagApproxTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        top->z_re = 0;
        top->z_im = 0;
    }
};

TEST_F(LogMagApproxTestbench, LogMagApprox0WorksTest)
{
    top->resetn = 0;

    top->eval();

    EXPECT_EQ(top->log2_mag, 0);
}

TEST_F(LogMagApproxTestbench, LogMagApprox1WorksTest)
{
    top->resetn = 1;
    top->z_re = 640;
    top->z_im = 480;

    for (int i = 0; i < 10; ++i) {
        runSimulation(1);
        printf("Cycle %d: log2_mag = %04x\n", i, top->log2_mag);
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