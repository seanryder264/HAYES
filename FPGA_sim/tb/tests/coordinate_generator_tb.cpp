/*
 *  Verifies the results of the CoordinateGenerator, exits with a 0 on success.
 */

#include "testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class CoordinateGeneratorTestbench : public Testbench
{
protected:
    void initializeInputs() override
    {
        top->clk = 1;
        top->resetn = 1;
        top->ready = 1;
    }
};

TEST_F(CoordinateGeneratorTestbench, CoordinateGenerator0WorksTest)
{
    runSimulation(1);
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