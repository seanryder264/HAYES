#include "Vpixel_generator.h"
#include "verilated.h"
#include <fstream>
#include <iostream>


int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vpixel_generator* top = new Vpixel_generator{contextp};

    std::ofstream output("coloured_function.ppm");

    output << "P3\n";
    output << 1024 << " " << 1024 << "\n";
    output << "255\n";

    top->axi_resetn = 0;
    top->periph_resetn = 0;
    top->out_stream_aclk = 0;
    top->s_axi_lite_aclk = 0;
    top->out_stream_tready = 1;

    for (int i = 0; i < 10; ++i) {
        top->out_stream_aclk = !top->out_stream_aclk;
        top->s_axi_lite_aclk = top->out_stream_aclk;
        top->eval();
    }

    top->axi_resetn = 1;
    top->periph_resetn = 1;

    const int max_pixels = 1024 * 1024;
    int pixels = 0;

    while (pixels < max_pixels) {

        top->out_stream_aclk = 1;
        top->s_axi_lite_aclk = 1;
        top->eval();

        uint8_t red   = top->r;
        uint8_t green = top->g;
        uint8_t blue  = top->b;

        output << (int)red << " " << (int)green << " " << (int)blue << "\n";
        pixels++;

        top->out_stream_aclk = 0;
        top->s_axi_lite_aclk = 0;
        top->eval();
    }

    output.close();
    delete top;
    return 0;
}