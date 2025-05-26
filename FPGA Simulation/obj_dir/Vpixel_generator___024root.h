// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vpixel_generator.h for the primary calling header

#ifndef VERILATED_VPIXEL_GENERATOR___024ROOT_H_
#define VERILATED_VPIXEL_GENERATOR___024ROOT_H_  // guard

#include "verilated.h"

class Vpixel_generator__Syms;

class Vpixel_generator___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(out_stream_aclk,0,0);
    VL_IN8(s_axi_lite_aclk,0,0);
    VL_IN8(axi_resetn,0,0);
    VL_IN8(periph_resetn,0,0);
    VL_OUT8(out_stream_tkeep,3,0);
    VL_OUT8(out_stream_tlast,0,0);
    VL_IN8(out_stream_tready,0,0);
    VL_OUT8(out_stream_tvalid,0,0);
    VL_OUT8(out_stream_tuser,0,0);
    VL_IN8(s_axi_lite_araddr,7,0);
    VL_OUT8(s_axi_lite_arready,0,0);
    VL_IN8(s_axi_lite_arvalid,0,0);
    VL_IN8(s_axi_lite_awaddr,7,0);
    VL_OUT8(s_axi_lite_awready,0,0);
    VL_IN8(s_axi_lite_awvalid,0,0);
    VL_IN8(s_axi_lite_bready,0,0);
    VL_OUT8(s_axi_lite_bresp,1,0);
    VL_OUT8(s_axi_lite_bvalid,0,0);
    VL_IN8(s_axi_lite_rready,0,0);
    VL_OUT8(s_axi_lite_rresp,1,0);
    VL_OUT8(s_axi_lite_rvalid,0,0);
    VL_OUT8(s_axi_lite_wready,0,0);
    VL_IN8(s_axi_lite_wvalid,0,0);
    CData/*2:0*/ pixel_generator__DOT__writeAddr;
    CData/*2:0*/ pixel_generator__DOT__readAddr;
    CData/*1:0*/ pixel_generator__DOT__readState;
    CData/*2:0*/ pixel_generator__DOT__writeState;
    CData/*0:0*/ pixel_generator__DOT__first;
    CData/*0:0*/ pixel_generator__DOT__lastx;
    CData/*0:0*/ pixel_generator__DOT__lasty;
    CData/*7:0*/ pixel_generator__DOT__r;
    CData/*7:0*/ pixel_generator__DOT__g;
    CData/*7:0*/ pixel_generator__DOT__b;
    CData/*1:0*/ pixel_generator__DOT__pixel_packer__DOT__state_reg;
    CData/*1:0*/ pixel_generator__DOT__pixel_packer__DOT__state;
    CData/*0:0*/ pixel_generator__DOT__pixel_packer__DOT__sof_reg;
    CData/*7:0*/ pixel_generator__DOT__pixel_packer__DOT__last_r;
    CData/*7:0*/ pixel_generator__DOT__pixel_packer__DOT__last_g;
    CData/*7:0*/ pixel_generator__DOT__pixel_packer__DOT__last_b;
    CData/*0:0*/ pixel_generator__DOT__pixel_packer__DOT__ready;
    CData/*0:0*/ __Vclklast__TOP__s_axi_lite_aclk;
    CData/*0:0*/ __Vclklast__TOP__out_stream_aclk;
    SData/*9:0*/ pixel_generator__DOT__x;
    SData/*8:0*/ pixel_generator__DOT__y;
    VL_OUT(out_stream_tdata,31,0);
    VL_OUT(s_axi_lite_rdata,31,0);
    VL_IN(s_axi_lite_wdata,31,0);
    IData/*31:0*/ pixel_generator__DOT__readData;
    IData/*31:0*/ pixel_generator__DOT__writeData;
    VlUnpacked<IData/*31:0*/, 8> pixel_generator__DOT__regfile;

    // INTERNAL VARIABLES
    Vpixel_generator__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vpixel_generator___024root(Vpixel_generator__Syms* symsp, const char* name);
    ~Vpixel_generator___024root();
    VL_UNCOPYABLE(Vpixel_generator___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
