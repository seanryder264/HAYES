// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpixel_generator.h for the primary calling header

#include "verilated.h"

#include "Vpixel_generator__Syms.h"
#include "Vpixel_generator___024root.h"

void Vpixel_generator___024root___ctor_var_reset(Vpixel_generator___024root* vlSelf);

Vpixel_generator___024root::Vpixel_generator___024root(Vpixel_generator__Syms* symsp, const char* name)
    : VerilatedModule{name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vpixel_generator___024root___ctor_var_reset(this);
}

void Vpixel_generator___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vpixel_generator___024root::~Vpixel_generator___024root() {
}
