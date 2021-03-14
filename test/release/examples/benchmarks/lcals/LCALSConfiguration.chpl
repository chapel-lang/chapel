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
  // Verification (or quick verification with only a few loop samples)
  //
  config const verify_checksums = true;
  config const verify_checksums_abbreviated = false;

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
  config const runA_pressureCalc = true;
  config const runA_energyCalc   = true;
  config const runA_vol3DCalc    = true;
  config const runA_delDotVec2D  = true;
  config const runA_couple       = true;
  config const runA_fir          = true;

  // Loop Subset B: "Basic" Loops.
  config const runB_init3     = true;
  config const runB_muladdsub = true;
  // Unstable, see https://github.com/chapel-lang/chapel/issues/11336
  config const runB_ifQuad    = false;
  config const runB_trapInt   = true;

  // Loop Subset C: Loops from older Livermore Loops in "C" suite.
  config const runC_hydro1D      = true;
  config const runC_iccg         = true;
  config const runC_innerProd    = true;
  config const runC_bandLinEq    = true;
  config const runC_tridiagElim  = true;
  config const runC_eos          = true;
  config const runC_adi          = true;
  config const runC_intPredict   = true;
  config const runC_diffPredict  = true;
  config const runC_firstSum     = true;
  config const runC_firstDiff    = true;
  config const runC_pic2D        = true;
  // Array OOB, see https://github.com/chapel-lang/chapel/issues/12806
  config const runC_pic1D        = false;
  config const runC_hydro2D      = true;
  config const runC_genLinRecur  = true;
  config const runC_discOrd      = true;
  config const runC_matXMat      = true;
  config const runC_planckian    = true;
  config const runC_impHydro2D   = true;
  config const runC_findFirstMin = true;

  //
  // Loop Variants
  //
  config const run_variantRaw              = true;
  config const run_variantForallLambda     = false; // not implemented
  config const run_variantRawOmp           = true;
  config const run_variantRawSPMD          = false; // off by default because
                                                    // short and medium loops
                                                    // take too long.
  config const run_variantRawVectorizeOnly = false; // off by default because
                                                    // it currently behaves
                                                    // identically to the
                                                    // serial variant
  config const run_variantForallLambdaOmp  = false; // not implemented
}
