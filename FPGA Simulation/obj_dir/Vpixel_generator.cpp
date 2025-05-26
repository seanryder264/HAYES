// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vpixel_generator.h"
#include "Vpixel_generator__Syms.h"

//============================================================
// Constructors

Vpixel_generator::Vpixel_generator(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vpixel_generator__Syms(contextp(), _vcname__, this)}
    , out_stream_aclk{vlSymsp->TOP.out_stream_aclk}
    , s_axi_lite_aclk{vlSymsp->TOP.s_axi_lite_aclk}
    , axi_resetn{vlSymsp->TOP.axi_resetn}
    , periph_resetn{vlSymsp->TOP.periph_resetn}
    , out_stream_tkeep{vlSymsp->TOP.out_stream_tkeep}
    , out_stream_tlast{vlSymsp->TOP.out_stream_tlast}
    , out_stream_tready{vlSymsp->TOP.out_stream_tready}
    , out_stream_tvalid{vlSymsp->TOP.out_stream_tvalid}
    , out_stream_tuser{vlSymsp->TOP.out_stream_tuser}
    , s_axi_lite_araddr{vlSymsp->TOP.s_axi_lite_araddr}
    , s_axi_lite_arready{vlSymsp->TOP.s_axi_lite_arready}
    , s_axi_lite_arvalid{vlSymsp->TOP.s_axi_lite_arvalid}
    , s_axi_lite_awaddr{vlSymsp->TOP.s_axi_lite_awaddr}
    , s_axi_lite_awready{vlSymsp->TOP.s_axi_lite_awready}
    , s_axi_lite_awvalid{vlSymsp->TOP.s_axi_lite_awvalid}
    , s_axi_lite_bready{vlSymsp->TOP.s_axi_lite_bready}
    , s_axi_lite_bresp{vlSymsp->TOP.s_axi_lite_bresp}
    , s_axi_lite_bvalid{vlSymsp->TOP.s_axi_lite_bvalid}
    , s_axi_lite_rready{vlSymsp->TOP.s_axi_lite_rready}
    , s_axi_lite_rresp{vlSymsp->TOP.s_axi_lite_rresp}
    , s_axi_lite_rvalid{vlSymsp->TOP.s_axi_lite_rvalid}
    , s_axi_lite_wready{vlSymsp->TOP.s_axi_lite_wready}
    , s_axi_lite_wvalid{vlSymsp->TOP.s_axi_lite_wvalid}
    , out_stream_tdata{vlSymsp->TOP.out_stream_tdata}
    , s_axi_lite_rdata{vlSymsp->TOP.s_axi_lite_rdata}
    , s_axi_lite_wdata{vlSymsp->TOP.s_axi_lite_wdata}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vpixel_generator::Vpixel_generator(const char* _vcname__)
    : Vpixel_generator(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vpixel_generator::~Vpixel_generator() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void Vpixel_generator___024root___eval_initial(Vpixel_generator___024root* vlSelf);
void Vpixel_generator___024root___eval_settle(Vpixel_generator___024root* vlSelf);
void Vpixel_generator___024root___eval(Vpixel_generator___024root* vlSelf);
#ifdef VL_DEBUG
void Vpixel_generator___024root___eval_debug_assertions(Vpixel_generator___024root* vlSelf);
#endif  // VL_DEBUG
void Vpixel_generator___024root___final(Vpixel_generator___024root* vlSelf);

static void _eval_initial_loop(Vpixel_generator__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    Vpixel_generator___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        Vpixel_generator___024root___eval_settle(&(vlSymsp->TOP));
        Vpixel_generator___024root___eval(&(vlSymsp->TOP));
    } while (0);
}

void Vpixel_generator::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vpixel_generator::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vpixel_generator___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        Vpixel_generator___024root___eval(&(vlSymsp->TOP));
    } while (0);
    // Evaluate cleanup
}

//============================================================
// Utilities

const char* Vpixel_generator::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

VL_ATTR_COLD void Vpixel_generator::final() {
    Vpixel_generator___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vpixel_generator::hierName() const { return vlSymsp->name(); }
const char* Vpixel_generator::modelName() const { return "Vpixel_generator"; }
unsigned Vpixel_generator::threads() const { return 1; }
