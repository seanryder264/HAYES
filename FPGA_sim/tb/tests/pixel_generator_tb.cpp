/*
 *  Verifies the results of the PixelGenerator, exits with a 0 on success.
 */

#include "base_testbench.h"
#include <fstream>

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

class PixelGeneratorTestbench : public BaseTestbench
{
protected:
    void initializeInputs() override
    {
        top->periph_resetn = 1;
        
        runSimulation(1);

        top->periph_resetn = 0;

        runSimulation(1);

        top->periph_resetn = 1;
        top->out_stream_tready = 1;
    }

    void runSimulation(int cycles = 1)
    {
        for (int i = 0; i < cycles; i++)
        {
            for (int clk = 0; clk < 2; clk++)
            {
                top->eval();
#ifndef __APPLE__
                tfp->dump(2 * ticks + clk);
#endif
                top->out_stream_aclk = !top->out_stream_aclk;
            }
            ticks++;

            if (Verilated::gotFinish())
            {
                exit(0);
            }
        }
    }
};

TEST_F(PixelGeneratorTestbench, PixelGenerator0WorksTest)
{
    std::ofstream output("coloured_function.ppm");

    const int WIDTH = 640;
    const int HEIGHT = 480;

    output << "P3\n";
    output << WIDTH << " " << HEIGHT << "\n";
    output << "255\n";

    const int max_pixels = WIDTH * HEIGHT;
    int pixels = 0;

    initializeInputs();

    while (pixels < max_pixels) {

        runSimulation(1);

        if (top->final_valid){
            uint8_t red   = top->r;
            uint8_t green = top->g;
            uint8_t blue  = top->b;

            output << (int)red << " " << (int)green << " " << (int)blue << "\n";
            pixels++;
        }
    }

    output.close();

    int ret = std::system("convert coloured_function.ppm coloured_function.png");
    if (ret != 0) {
        std::cerr << "Error: Could not convert PPM to PNG. Make sure ImageMagick is installed.\n";
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