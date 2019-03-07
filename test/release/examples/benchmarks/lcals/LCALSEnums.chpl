module LCALSEnums {
  enum LoopLength {
    LONG = 0,
    MEDIUM,
    SHORT,
  }

  enum LoopKernelID {
    // Keep this one first and don't comment out (!!)
    // This insures loop ids start at zero so all array indexing
    // or data structures is correct.  Also, this loop is not
    // executed the same way the others are.
    REF_LOOP = 0,

    //
    // Loop Subset A: Loops extracted from LLNL app codes.
    // They are implemented in runA<variant>Loops.cxx files.
    //
    PRESSURE_CALC,
    PRESSURE_CALC_ALT,
    ENERGY_CALC,
    ENERGY_CALC_ALT,
    VOL3D_CALC,
    DEL_DOT_VEC_2D,
    COUPLE,
    FIR,

    //
    // Loop Subset B: "Basic" Loops.
    // They are implemented in runB<variant>Loops.cxx files.
    //
    INIT3,
    MULADDSUB,
    IF_QUAD,
    TRAP_INT,

    //
    // Loop Subset C: Loops from older Livermore Loops in "C" suite.
    // They are implemented in runC<variant>Loops.cxx files.
    //
    HYDRO_1D,
    ICCG,
    INNER_PROD,
    BAND_LIN_EQ,
    TRIDIAG_ELIM,
    EOS,
    ADI,
    INT_PREDICT,
    DIFF_PREDICT,
    FIRST_SUM,
    FIRST_DIFF,
    PIC_2D,
    PIC_1D,
    HYDRO_2D,
    GEN_LIN_RECUR,
    DISC_ORD,
    MAT_X_MAT,
    PLANCKIAN,
    IMP_HYDRO_2D,
    FIND_FIRST_MIN
  }

  enum LoopVariantID {
    //
    // These variants define LCALS benchmark
    //
    RAW = 1,
    RAW_OMP,
    RAW_SPMD,
    RAW_VECTOR_ONLY,
    FORALL_LAMBDA,
    FORALL_LAMBDA_OMP,

    // ifdef LCALS_DO_MISC
    FORALL_HYBRID_LAMBDA,
    FORALL_FUNCTOR,
    FORALL_FUNCTOR_OMP,

    RAW_FUNC,
    FORALL_LAMBDA_TYPEFIX,
    FORALL_LAMBDA_OMP_TYPEFIX,
    FORALL_HYBRID_LAMBDA_TYPEFIX
  }

  enum WeightGroup {
    DATA_PARALLEL = 0,
    ORDER_DEPENDENT,
    TRANSCENDENTAL,
    DATA_DEPENDENT,
    POINTER_NEST,
    COMPLEX,
    NUM_WEIGHT_GROUPS
  }

  enum OutputStyle {
    MINIMAL,   // no timing/checksum output
    REFERENCE, // output matching the reference code
    PERF_TEST  // output formatted for perf testing
  }
}
