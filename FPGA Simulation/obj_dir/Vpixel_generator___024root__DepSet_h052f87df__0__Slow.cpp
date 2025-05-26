// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpixel_generator.h for the primary calling header

#include "verilated.h"

#include "Vpixel_generator___024root.h"

VL_ATTR_COLD void Vpixel_generator___024root___initial__TOP__0(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___initial__TOP__0\n"); );
    // Body
    vlSelf->s_axi_lite_rresp = 0U;
    vlSelf->s_axi_lite_bresp = 2U;
    vlSelf->out_stream_tkeep = 0xfU;
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg = 0U;
    vlSelf->pixel_generator__DOT__writeState = 0U;
    vlSelf->pixel_generator__DOT__readState = 0U;
}

VL_ATTR_COLD void Vpixel_generator___024root___settle__TOP__0(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___settle__TOP__0\n"); );
    // Init
    IData/*31:0*/ pixel_generator__DOT__pixel_packer__DOT__tdata;
    CData/*0:0*/ pixel_generator__DOT__pixel_packer__DOT__tvalid;
    // Body
    vlSelf->s_axi_lite_rdata = vlSelf->pixel_generator__DOT__readData;
    vlSelf->pixel_generator__DOT__lastx = (0x27fU == (IData)(vlSelf->pixel_generator__DOT__x));
    vlSelf->pixel_generator__DOT__lasty = (0x1dfU == (IData)(vlSelf->pixel_generator__DOT__y));
    vlSelf->out_stream_tlast = (0x27fU == (IData)(vlSelf->pixel_generator__DOT__x));
    vlSelf->out_stream_tuser = vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg;
    vlSelf->pixel_generator__DOT__r = (0xffU & ((IData)(vlSelf->pixel_generator__DOT__x) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    vlSelf->pixel_generator__DOT__g = (0xffU & ((IData)(vlSelf->pixel_generator__DOT__y) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    vlSelf->pixel_generator__DOT__b = (0xffU & (((0x7fU 
                                                  & (IData)(vlSelf->pixel_generator__DOT__x)) 
                                                 + 
                                                 (0x7fU 
                                                  & (IData)(vlSelf->pixel_generator__DOT__y))) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    vlSelf->pixel_generator__DOT__first = ((0U == (IData)(vlSelf->pixel_generator__DOT__x)) 
                                           & (0U == (IData)(vlSelf->pixel_generator__DOT__y)));
    vlSelf->s_axi_lite_awready = ((0U == (IData)(vlSelf->pixel_generator__DOT__writeState)) 
                                  | (2U == (IData)(vlSelf->pixel_generator__DOT__writeState)));
    vlSelf->s_axi_lite_wready = ((0U == (IData)(vlSelf->pixel_generator__DOT__writeState)) 
                                 | (1U == (IData)(vlSelf->pixel_generator__DOT__writeState)));
    vlSelf->s_axi_lite_bvalid = (5U == (IData)(vlSelf->pixel_generator__DOT__writeState));
    vlSelf->s_axi_lite_arready = (0U == (IData)(vlSelf->pixel_generator__DOT__readState));
    vlSelf->s_axi_lite_rvalid = (2U == (IData)(vlSelf->pixel_generator__DOT__readState));
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__state 
        = ((IData)(vlSelf->pixel_generator__DOT__first)
            ? 0U : (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg));
    if ((2U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))) {
        vlSelf->pixel_generator__DOT__pixel_packer__DOT__ready 
            = (1U & (IData)(vlSelf->out_stream_tready));
        pixel_generator__DOT__pixel_packer__DOT__tdata 
            = ((1U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
                ? (((IData)(vlSelf->pixel_generator__DOT__r) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__b) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__g) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r))))
                : (((IData)(vlSelf->pixel_generator__DOT__b) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__g) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b)))));
    } else {
        vlSelf->pixel_generator__DOT__pixel_packer__DOT__ready 
            = (1U & ((~ (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state)) 
                     | (IData)(vlSelf->out_stream_tready)));
        pixel_generator__DOT__pixel_packer__DOT__tdata 
            = ((1U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
                ? (((IData)(vlSelf->pixel_generator__DOT__g) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g))))
                : (((IData)(vlSelf->pixel_generator__DOT__g) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g)))));
    }
    pixel_generator__DOT__pixel_packer__DOT__tvalid 
        = (IData)((0U != (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state)));
    vlSelf->out_stream_tvalid = pixel_generator__DOT__pixel_packer__DOT__tvalid;
    vlSelf->out_stream_tdata = pixel_generator__DOT__pixel_packer__DOT__tdata;
}

VL_ATTR_COLD void Vpixel_generator___024root___eval_initial(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___eval_initial\n"); );
    // Body
    Vpixel_generator___024root___initial__TOP__0(vlSelf);
    vlSelf->__Vclklast__TOP__s_axi_lite_aclk = vlSelf->s_axi_lite_aclk;
    vlSelf->__Vclklast__TOP__out_stream_aclk = vlSelf->out_stream_aclk;
}

VL_ATTR_COLD void Vpixel_generator___024root___eval_settle(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___eval_settle\n"); );
    // Body
    Vpixel_generator___024root___settle__TOP__0(vlSelf);
}

VL_ATTR_COLD void Vpixel_generator___024root___final(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___final\n"); );
}

VL_ATTR_COLD void Vpixel_generator___024root___ctor_var_reset(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->out_stream_aclk = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_aclk = VL_RAND_RESET_I(1);
    vlSelf->axi_resetn = VL_RAND_RESET_I(1);
    vlSelf->periph_resetn = VL_RAND_RESET_I(1);
    vlSelf->out_stream_tdata = VL_RAND_RESET_I(32);
    vlSelf->out_stream_tkeep = VL_RAND_RESET_I(4);
    vlSelf->out_stream_tlast = VL_RAND_RESET_I(1);
    vlSelf->out_stream_tready = VL_RAND_RESET_I(1);
    vlSelf->out_stream_tvalid = VL_RAND_RESET_I(1);
    vlSelf->out_stream_tuser = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_araddr = VL_RAND_RESET_I(8);
    vlSelf->s_axi_lite_arready = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_arvalid = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_awaddr = VL_RAND_RESET_I(8);
    vlSelf->s_axi_lite_awready = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_awvalid = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_bready = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_bresp = VL_RAND_RESET_I(2);
    vlSelf->s_axi_lite_bvalid = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_rdata = VL_RAND_RESET_I(32);
    vlSelf->s_axi_lite_rready = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_rresp = VL_RAND_RESET_I(2);
    vlSelf->s_axi_lite_rvalid = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_wdata = VL_RAND_RESET_I(32);
    vlSelf->s_axi_lite_wready = VL_RAND_RESET_I(1);
    vlSelf->s_axi_lite_wvalid = VL_RAND_RESET_I(1);
    for (int __Vi0=0; __Vi0<8; ++__Vi0) {
        vlSelf->pixel_generator__DOT__regfile[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->pixel_generator__DOT__writeAddr = VL_RAND_RESET_I(3);
    vlSelf->pixel_generator__DOT__readAddr = VL_RAND_RESET_I(3);
    vlSelf->pixel_generator__DOT__readData = VL_RAND_RESET_I(32);
    vlSelf->pixel_generator__DOT__writeData = VL_RAND_RESET_I(32);
    vlSelf->pixel_generator__DOT__readState = VL_RAND_RESET_I(2);
    vlSelf->pixel_generator__DOT__writeState = VL_RAND_RESET_I(3);
    vlSelf->pixel_generator__DOT__x = VL_RAND_RESET_I(10);
    vlSelf->pixel_generator__DOT__y = VL_RAND_RESET_I(9);
    vlSelf->pixel_generator__DOT__first = VL_RAND_RESET_I(1);
    vlSelf->pixel_generator__DOT__lastx = VL_RAND_RESET_I(1);
    vlSelf->pixel_generator__DOT__lasty = VL_RAND_RESET_I(1);
    vlSelf->pixel_generator__DOT__r = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__g = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__b = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg = VL_RAND_RESET_I(2);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__state = VL_RAND_RESET_I(2);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg = VL_RAND_RESET_I(1);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b = VL_RAND_RESET_I(8);
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__ready = VL_RAND_RESET_I(1);
}
