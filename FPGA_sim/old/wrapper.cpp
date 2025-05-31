#include "Vpixel_generator.h"
#include "verilated.h"
#include <fstream>
#include <iostream>


int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vpixel_generator* top = new Vpixel_generator{contextp};

    std::ofstream raw_data("data_stream.bin", std::ios::binary);

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

    const int max_pixels = 640 * 480;
    int x = 0, y = 0;
    bool sof = false;

    for (int cycle = 0; cycle < 350000; cycle++) {
        top->out_stream_aclk = 1;
        top->s_axi_lite_aclk = 1;
        top->eval();

        if (top->out_stream_tuser == 1) {
            sof = true;
        }

        if (sof && top->out_stream_tvalid && top->out_stream_tready) {
            uint32_t word = top->out_stream_tdata;
            raw_data.write(reinterpret_cast<const char*>(&word), sizeof(word));
        }

        top->out_stream_aclk = 0;
        top->s_axi_lite_aclk = 0;
        top->eval();
    }

    delete top;
    return 0;
}