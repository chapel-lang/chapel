module LCALSConfiguration {

  //
  // Output directory, use stdout if left empty
  //
  config const output_dirname = "";

  //
  // Do Figure Of Merit calculation?
  //
  config const do_fom = false;

  //
  // Number of passes to run for each kernel and length
  //
  config const num_suite_passes = 1;

  //
  // Loop lengths
  //
  config const run_shortLoops  = true;
  config const run_mediumLoops = true;
  config const run_longLoops   = true;

  //
  // Loop Kernels
  //
  config const run_refLoop = true;

  // Loop Subset A: Loops extracted from LLNL app codes.
  config const runA_pressureCalc = false; // M and S have bad checksums
  config const runA_energyCalc   = true;
  config const runA_vol3DCalc    = true;
  config const runA_delDotVec2D  = true;
  config const runA_couple       = true;
  config const runA_fir          = false; // bad checksums

  // Loop Subset B: "Basic" Loops.
  config const runB_init3     = true;
  config const runB_muladdsub = true;
  config const runB_ifQuad    = true;
  config const runB_trapInt   = true;

  // Loop Subset C: Loops from older Livermore Loops in "C" suite.
  config const runC_hydro1D      = true;
  config const runC_iccg         = false; // segfaults
  config const runC_innerProd    = true;
  config const runC_bandLinEq    = false; // bad checksums
  config const runC_tridiagElim  = false; // bad checksums
  config const runC_eos          = false; // bad checksums
  config const runC_adi          = false; // not implemented
  config const runC_intPredict   = false; // not implemented
  config const runC_diffPredict  = false; // not implemented
  config const runC_firstSum     = false; // ???
  config const runC_firstDiff    = false; // bad checksums
  config const runC_pic2D        = false; // not implemented
  config const runC_pic1D        = false; // bad checksums
  config const runC_hydro2D      = false; // not implemented
  config const runC_genLinRecur  = false; // bad checksums
  config const runC_discOrd      = false; // bad checksums
  config const runC_matXMat      = false; // not implemented
  config const runC_planckian    = false; // bad checksums
  config const runC_impHydro2D   = false; // not implemented
  config const runC_findFirstMin = true;

  //
  // Loop Variants
  //
  config const run_variantRaw              = true;
  config const run_variantForallLambda     = false; // not implemented
  config const run_variantRawOmp           = true;
  config const run_variantRawVectorizeOnly = true;
  config const run_variantForallLambdaOmp  = false; // not implemented
}
