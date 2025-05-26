#include "Vpixel_generator.h"
#include "verilated.h"

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vpixle_generator* top = new Vpixle_generator{contextp};
    while (!contextp->gotFinish()) { top->eval(); }

    std::ofstream out("out_stream_log.csv");
    out << "time,tdata,valid,ready,last,sof\n";

    top->axi_resetn = 0;
    top->periph_resetn = 0;
    top->out_stream_aclk = 0;
    top->s_axi_lite_aclk = 0;

    top->out_stream_tready = 1;  // Always ready to accept data

    // Simulate 5 clock cycles of reset
    for (int i = 0; i < 10; i++) {
        top->out_stream_aclk = !top->out_stream_aclk;
        top->s_axi_lite_aclk = top->out_stream_aclk;
        top->eval();
        main_time++;
    }

    top->axi_resetn = 1;
    top->periph_resetn = 1;

    // Main simulation loop
    for (int cycle = 0; cycle < 2000; cycle++) {
        // Rising edge
        top->out_stream_aclk = 1;
        top->s_axi_lite_aclk = 1;
        top->eval();
        main_time++;

        // Capture output if valid & ready
        if (top->out_stream_tvalid && top->out_stream_tready) {
            out << main_time << ","
                << top->out_stream_tdata << ","
                << (int)top->out_stream_tvalid << ","
                << (int)top->out_stream_tready << ","
                << (int)top->out_stream_tlast << ","
                << (int)top->out_stream_tuser << "\n";
        }

        // Falling edge
        top->out_stream_aclk = 0;
        top->s_axi_lite_aclk = 0;
        top->eval();
        main_time++;
    }

    out.close();
    delete top;
    return 0;
}
  }

  