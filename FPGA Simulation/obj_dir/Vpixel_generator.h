// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VPIXEL_GENERATOR_H_
#define VERILATED_VPIXEL_GENERATOR_H_  // guard

#include "verilated.h"

class Vpixel_generator__Syms;
class Vpixel_generator___024root;

// This class is the main interface to the Verilated model
class Vpixel_generator VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vpixel_generator__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&out_stream_aclk,0,0);
    VL_IN8(&s_axi_lite_aclk,0,0);
    VL_IN8(&axi_resetn,0,0);
    VL_IN8(&periph_resetn,0,0);
    VL_OUT8(&out_stream_tkeep,3,0);
    VL_OUT8(&out_stream_tlast,0,0);
    VL_IN8(&out_stream_tready,0,0);
    VL_OUT8(&out_stream_tvalid,0,0);
    VL_OUT8(&out_stream_tuser,0,0);
    VL_IN8(&s_axi_lite_araddr,7,0);
    VL_OUT8(&s_axi_lite_arready,0,0);
    VL_IN8(&s_axi_lite_arvalid,0,0);
    VL_IN8(&s_axi_lite_awaddr,7,0);
    VL_OUT8(&s_axi_lite_awready,0,0);
    VL_IN8(&s_axi_lite_awvalid,0,0);
    VL_IN8(&s_axi_lite_bready,0,0);
    VL_OUT8(&s_axi_lite_bresp,1,0);
    VL_OUT8(&s_axi_lite_bvalid,0,0);
    VL_IN8(&s_axi_lite_rready,0,0);
    VL_OUT8(&s_axi_lite_rresp,1,0);
    VL_OUT8(&s_axi_lite_rvalid,0,0);
    VL_OUT8(&s_axi_lite_wready,0,0);
    VL_IN8(&s_axi_lite_wvalid,0,0);
    VL_OUT(&out_stream_tdata,31,0);
    VL_OUT(&s_axi_lite_rdata,31,0);
    VL_IN(&s_axi_lite_wdata,31,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vpixel_generator___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vpixel_generator(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vpixel_generator(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vpixel_generator();
  private:
    VL_UNCOPYABLE(Vpixel_generator);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
