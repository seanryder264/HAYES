#include "Vpixel_generator.h"
#include "verilated.h"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vpixel_generator* top = new Vpixel_generator{contextp};

    std::ofstream out("out_image.ppm");
    out << "P3\n";
    out << "640 480\n";
    out << "255\n";

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
    int pixel_count = 0;

    std::vector<uint32_t> buffer;

    for (int cycle = 0; cycle < 400000 && pixel_count < max_pixels; cycle++) {
        top->out_stream_aclk = 1;
        top->s_axi_lite_aclk = 1;
        top->eval();

        if (top->out_stream_tvalid && top->out_stream_tready) {
            buffer.push_back(static_cast<uint32_t>(top->out_stream_tdata));

            if (buffer.size() == 3) {
                uint32_t w1 = buffer[0];
                uint32_t w2 = buffer[1];
                uint32_t w3 = buffer[2];

                // Extract and write each of the 4 pixels
                uint8_t r0 = (w1 >> 24) & 0xFF;
                uint8_t g0 = (w1 >> 16) & 0xFF;
                uint8_t b0 = (w1 >> 8)  & 0xFF;
                uint8_t r1 = (w1)       & 0xFF;

                uint8_t g1 = (w2 >> 24) & 0xFF;
                uint8_t b1 = (w2 >> 16) & 0xFF;
                uint8_t r2 = (w2 >> 8)  & 0xFF;
                uint8_t g2 = (w2)       & 0xFF;

                uint8_t b2 = (w3 >> 24) & 0xFF;
                uint8_t r3 = (w3 >> 16) & 0xFF;
                uint8_t g3 = (w3 >> 8)  & 0xFF;
                uint8_t b3 = (w3)       & 0xFF;

                auto write_pixel = [&](uint8_t r, uint8_t g, uint8_t b) {
                    if (pixel_count < max_pixels) {
                        out << (int)r << " " << (int)g << " " << (int)b << "\n";
                        pixel_count++;
                    }
                };

                write_pixel(r0, g0, b0);
                write_pixel(r1, g1, b1);
                write_pixel(r2, g2, b2);
                write_pixel(r3, g3, b3);

                buffer.clear();
            }
        }

        top->out_stream_aclk = 0;
        top->s_axi_lite_aclk = 0;
        top->eval();
    }

    out.close();
    delete top;
    return 0;
}