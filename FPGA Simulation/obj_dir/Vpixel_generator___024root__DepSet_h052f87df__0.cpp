// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpixel_generator.h for the primary calling header

#include "verilated.h"

#include "Vpixel_generator___024root.h"

VL_INLINE_OPT void Vpixel_generator___024root___sequent__TOP__0(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___sequent__TOP__0\n"); );
    // Init
    CData/*1:0*/ __Vdly__pixel_generator__DOT__readState;
    CData/*2:0*/ __Vdly__pixel_generator__DOT__writeState;
    CData/*2:0*/ __Vdlyvdim0__pixel_generator__DOT__regfile__v0;
    IData/*31:0*/ __Vdlyvval__pixel_generator__DOT__regfile__v0;
    CData/*0:0*/ __Vdlyvset__pixel_generator__DOT__regfile__v0;
    // Body
    __Vdly__pixel_generator__DOT__readState = vlSelf->pixel_generator__DOT__readState;
    __Vdly__pixel_generator__DOT__writeState = vlSelf->pixel_generator__DOT__writeState;
    __Vdlyvset__pixel_generator__DOT__regfile__v0 = 0U;
    vlSelf->pixel_generator__DOT__readData = vlSelf->pixel_generator__DOT__regfile
        [vlSelf->pixel_generator__DOT__readAddr];
    if (vlSelf->axi_resetn) {
        if ((4U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
            if ((2U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
                __Vdly__pixel_generator__DOT__writeState = 0U;
            } else if ((1U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
                if (vlSelf->s_axi_lite_bready) {
                    __Vdly__pixel_generator__DOT__writeState = 0U;
                }
            } else {
                __Vdlyvval__pixel_generator__DOT__regfile__v0 
                    = vlSelf->pixel_generator__DOT__writeData;
                __Vdlyvset__pixel_generator__DOT__regfile__v0 = 1U;
                __Vdlyvdim0__pixel_generator__DOT__regfile__v0 
                    = vlSelf->pixel_generator__DOT__writeAddr;
                __Vdly__pixel_generator__DOT__writeState = 5U;
            }
        } else if ((2U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
            if ((1U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
                __Vdly__pixel_generator__DOT__writeState = 0U;
            } else if (vlSelf->s_axi_lite_awvalid) {
                vlSelf->pixel_generator__DOT__writeAddr 
                    = (7U & ((IData)(vlSelf->s_axi_lite_awaddr) 
                             >> 2U));
                __Vdly__pixel_generator__DOT__writeState = 4U;
            }
        } else if ((1U & (IData)(vlSelf->pixel_generator__DOT__writeState))) {
            if (vlSelf->s_axi_lite_wvalid) {
                vlSelf->pixel_generator__DOT__writeData 
                    = vlSelf->s_axi_lite_wdata;
                __Vdly__pixel_generator__DOT__writeState = 4U;
            }
        } else if ((2U == (((IData)(vlSelf->s_axi_lite_awvalid) 
                            << 1U) | (IData)(vlSelf->s_axi_lite_wvalid)))) {
            vlSelf->pixel_generator__DOT__writeAddr 
                = (7U & ((IData)(vlSelf->s_axi_lite_awaddr) 
                         >> 2U));
            __Vdly__pixel_generator__DOT__writeState = 1U;
        } else if ((1U == (((IData)(vlSelf->s_axi_lite_awvalid) 
                            << 1U) | (IData)(vlSelf->s_axi_lite_wvalid)))) {
            vlSelf->pixel_generator__DOT__writeData 
                = vlSelf->s_axi_lite_wdata;
            __Vdly__pixel_generator__DOT__writeState = 2U;
        } else if ((3U == (((IData)(vlSelf->s_axi_lite_awvalid) 
                            << 1U) | (IData)(vlSelf->s_axi_lite_wvalid)))) {
            vlSelf->pixel_generator__DOT__writeData 
                = vlSelf->s_axi_lite_wdata;
            vlSelf->pixel_generator__DOT__writeAddr 
                = (7U & ((IData)(vlSelf->s_axi_lite_awaddr) 
                         >> 2U));
            __Vdly__pixel_generator__DOT__writeState = 4U;
        } else {
            __Vdly__pixel_generator__DOT__writeState = 0U;
        }
        if ((0U == (IData)(vlSelf->pixel_generator__DOT__readState))) {
            if (vlSelf->s_axi_lite_arvalid) {
                vlSelf->pixel_generator__DOT__readAddr 
                    = (7U & ((IData)(vlSelf->s_axi_lite_araddr) 
                             >> 2U));
                __Vdly__pixel_generator__DOT__readState = 1U;
            }
        } else if ((1U == (IData)(vlSelf->pixel_generator__DOT__readState))) {
            __Vdly__pixel_generator__DOT__readState = 2U;
        } else if ((2U == (IData)(vlSelf->pixel_generator__DOT__readState))) {
            if (vlSelf->s_axi_lite_rready) {
                __Vdly__pixel_generator__DOT__readState = 0U;
            }
        } else {
            __Vdly__pixel_generator__DOT__readState = 0U;
        }
    } else {
        __Vdly__pixel_generator__DOT__writeState = 0U;
        __Vdly__pixel_generator__DOT__readState = 0U;
    }
    vlSelf->pixel_generator__DOT__writeState = __Vdly__pixel_generator__DOT__writeState;
    if (__Vdlyvset__pixel_generator__DOT__regfile__v0) {
        vlSelf->pixel_generator__DOT__regfile[__Vdlyvdim0__pixel_generator__DOT__regfile__v0] 
            = __Vdlyvval__pixel_generator__DOT__regfile__v0;
    }
    vlSelf->s_axi_lite_rdata = vlSelf->pixel_generator__DOT__readData;
    vlSelf->s_axi_lite_awready = ((0U == (IData)(vlSelf->pixel_generator__DOT__writeState)) 
                                  | (2U == (IData)(vlSelf->pixel_generator__DOT__writeState)));
    vlSelf->s_axi_lite_wready = ((0U == (IData)(vlSelf->pixel_generator__DOT__writeState)) 
                                 | (1U == (IData)(vlSelf->pixel_generator__DOT__writeState)));
    vlSelf->s_axi_lite_bvalid = (5U == (IData)(vlSelf->pixel_generator__DOT__writeState));
    vlSelf->pixel_generator__DOT__readState = __Vdly__pixel_generator__DOT__readState;
    vlSelf->s_axi_lite_arready = (0U == (IData)(vlSelf->pixel_generator__DOT__readState));
    vlSelf->s_axi_lite_rvalid = (2U == (IData)(vlSelf->pixel_generator__DOT__readState));
}

VL_INLINE_OPT void Vpixel_generator___024root___sequent__TOP__1(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___sequent__TOP__1\n"); );
    // Init
    CData/*0:0*/ pixel_generator__DOT__pixel_packer__DOT__tvalid;
    SData/*9:0*/ __Vdly__pixel_generator__DOT__x;
    // Body
    __Vdly__pixel_generator__DOT__x = vlSelf->pixel_generator__DOT__x;
    if (vlSelf->periph_resetn) {
        if (vlSelf->pixel_generator__DOT__first) {
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg = 1U;
        } else if (vlSelf->out_stream_tready) {
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg = 0U;
        }
        if (((0U == (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state)) 
             | (IData)(vlSelf->out_stream_tready))) {
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g 
                = vlSelf->pixel_generator__DOT__g;
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r 
                = vlSelf->pixel_generator__DOT__r;
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b 
                = vlSelf->pixel_generator__DOT__b;
            vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg 
                = ((0x27fU == (IData)(vlSelf->pixel_generator__DOT__x))
                    ? 0U : (3U & ((IData)(1U) + (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))));
        }
        if (vlSelf->pixel_generator__DOT__pixel_packer__DOT__ready) {
            __Vdly__pixel_generator__DOT__x = ((IData)(vlSelf->pixel_generator__DOT__lastx)
                                                ? 0U
                                                : (0x3ffU 
                                                   & ((IData)(1U) 
                                                      + (IData)(vlSelf->pixel_generator__DOT__x))));
            if ((0x27fU == (IData)(vlSelf->pixel_generator__DOT__x))) {
                vlSelf->pixel_generator__DOT__y = ((IData)(vlSelf->pixel_generator__DOT__lasty)
                                                    ? 0U
                                                    : 
                                                   (0x1ffU 
                                                    & ((IData)(1U) 
                                                       + (IData)(vlSelf->pixel_generator__DOT__y))));
            }
        }
    } else {
        vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg = 0U;
        vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg = 0U;
        __Vdly__pixel_generator__DOT__x = 0U;
        vlSelf->pixel_generator__DOT__y = 0U;
    }
    vlSelf->pixel_generator__DOT__x = __Vdly__pixel_generator__DOT__x;
    vlSelf->out_stream_tuser = vlSelf->pixel_generator__DOT__pixel_packer__DOT__sof_reg;
    vlSelf->pixel_generator__DOT__lasty = (0x1dfU == (IData)(vlSelf->pixel_generator__DOT__y));
    vlSelf->pixel_generator__DOT__lastx = (0x27fU == (IData)(vlSelf->pixel_generator__DOT__x));
    vlSelf->out_stream_tlast = (0x27fU == (IData)(vlSelf->pixel_generator__DOT__x));
    vlSelf->pixel_generator__DOT__first = ((0U == (IData)(vlSelf->pixel_generator__DOT__x)) 
                                           & (0U == (IData)(vlSelf->pixel_generator__DOT__y)));
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__state 
        = ((IData)(vlSelf->pixel_generator__DOT__first)
            ? 0U : (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state_reg));
    pixel_generator__DOT__pixel_packer__DOT__tvalid 
        = (IData)((0U != (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state)));
    vlSelf->out_stream_tvalid = pixel_generator__DOT__pixel_packer__DOT__tvalid;
}

VL_INLINE_OPT void Vpixel_generator___024root___multiclk__TOP__0(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___multiclk__TOP__0\n"); );
    // Init
    IData/*31:0*/ pixel_generator__DOT__pixel_packer__DOT__tdata;
    // Body
    vlSelf->pixel_generator__DOT__g = (0xffU & ((IData)(vlSelf->pixel_generator__DOT__y) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    vlSelf->pixel_generator__DOT__r = (0xffU & ((IData)(vlSelf->pixel_generator__DOT__x) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    vlSelf->pixel_generator__DOT__b = (0xffU & (((0x7fU 
                                                  & (IData)(vlSelf->pixel_generator__DOT__x)) 
                                                 + 
                                                 (0x7fU 
                                                  & (IData)(vlSelf->pixel_generator__DOT__y))) 
                                                + vlSelf->pixel_generator__DOT__regfile
                                                [0U]));
    pixel_generator__DOT__pixel_packer__DOT__tdata 
        = ((2U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
            ? ((1U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
                ? (((IData)(vlSelf->pixel_generator__DOT__r) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__b) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__g) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r))))
                : (((IData)(vlSelf->pixel_generator__DOT__b) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__g) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b)))))
            : ((1U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
                ? (((IData)(vlSelf->pixel_generator__DOT__g) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g))))
                : (((IData)(vlSelf->pixel_generator__DOT__g) 
                    << 0x18U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_r) 
                                  << 0x10U) | (((IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_b) 
                                                << 8U) 
                                               | (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__last_g))))));
    vlSelf->out_stream_tdata = pixel_generator__DOT__pixel_packer__DOT__tdata;
}

VL_INLINE_OPT void Vpixel_generator___024root___combo__TOP__0(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___combo__TOP__0\n"); );
    // Body
    vlSelf->pixel_generator__DOT__pixel_packer__DOT__ready 
        = (1U & ((2U & (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state))
                  ? (IData)(vlSelf->out_stream_tready)
                  : ((~ (IData)(vlSelf->pixel_generator__DOT__pixel_packer__DOT__state)) 
                     | (IData)(vlSelf->out_stream_tready))));
}

void Vpixel_generator___024root___eval(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___eval\n"); );
    // Body
    if (((IData)(vlSelf->s_axi_lite_aclk) & (~ (IData)(vlSelf->__Vclklast__TOP__s_axi_lite_aclk)))) {
        Vpixel_generator___024root___sequent__TOP__0(vlSelf);
    }
    if (((IData)(vlSelf->out_stream_aclk) & (~ (IData)(vlSelf->__Vclklast__TOP__out_stream_aclk)))) {
        Vpixel_generator___024root___sequent__TOP__1(vlSelf);
    }
    if ((((IData)(vlSelf->out_stream_aclk) & (~ (IData)(vlSelf->__Vclklast__TOP__out_stream_aclk))) 
         | ((IData)(vlSelf->s_axi_lite_aclk) & (~ (IData)(vlSelf->__Vclklast__TOP__s_axi_lite_aclk))))) {
        Vpixel_generator___024root___multiclk__TOP__0(vlSelf);
    }
    Vpixel_generator___024root___combo__TOP__0(vlSelf);
    // Final
    vlSelf->__Vclklast__TOP__s_axi_lite_aclk = vlSelf->s_axi_lite_aclk;
    vlSelf->__Vclklast__TOP__out_stream_aclk = vlSelf->out_stream_aclk;
}

#ifdef VL_DEBUG
void Vpixel_generator___024root___eval_debug_assertions(Vpixel_generator___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vpixel_generator__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpixel_generator___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->out_stream_aclk & 0xfeU))) {
        Verilated::overWidthError("out_stream_aclk");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_aclk & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_aclk");}
    if (VL_UNLIKELY((vlSelf->axi_resetn & 0xfeU))) {
        Verilated::overWidthError("axi_resetn");}
    if (VL_UNLIKELY((vlSelf->periph_resetn & 0xfeU))) {
        Verilated::overWidthError("periph_resetn");}
    if (VL_UNLIKELY((vlSelf->out_stream_tready & 0xfeU))) {
        Verilated::overWidthError("out_stream_tready");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_arvalid & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_arvalid");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_awvalid & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_awvalid");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_bready & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_bready");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_rready & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_rready");}
    if (VL_UNLIKELY((vlSelf->s_axi_lite_wvalid & 0xfeU))) {
        Verilated::overWidthError("s_axi_lite_wvalid");}
}
#endif  // VL_DEBUG
