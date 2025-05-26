// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VPIXEL_GENERATOR__SYMS_H_
#define VERILATED_VPIXEL_GENERATOR__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vpixel_generator.h"

// INCLUDE MODULE CLASSES
#include "Vpixel_generator___024root.h"

// SYMS CLASS (contains all model state)
class Vpixel_generator__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vpixel_generator* const __Vm_modelp;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vpixel_generator___024root     TOP;

    // CONSTRUCTORS
    Vpixel_generator__Syms(VerilatedContext* contextp, const char* namep, Vpixel_generator* modelp);
    ~Vpixel_generator__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
