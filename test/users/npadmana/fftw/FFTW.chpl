// Wrapper for FFTW version 3.
module FFTW {

  use SysCTypes;

  // Define the various planner flags
  // See Sec. 4.3.2 of FFTW manual "Planner Flags"
  extern const FFTW_FORWARD : c_int;
  extern const FFTW_BACKWARD : c_int;
  extern const FFTW_ESTIMATE : c_uint;
  extern const FFTW_MEASURE : c_uint;
  extern const FFTW_PATIENT : c_uint;
  extern const FFTW_EXHAUSTIVE : c_uint;
  extern const FFTW_WISDOM_ONLY : c_uint;
  extern const FFTW_DESTROY_INPUT : c_uint;
  extern const FFTW_PRESERVE_INPUT : c_uint;
  extern const FFTW_UNALIGNED : c_uint;

  // Define types
  // NOTE : Ideally, this should be mapped to complex(128), but I seem to run into 
  // casting issues in the C code. This is a simple workaround for now - although ugly.
  // Also note that if one used complex types, then one would need to include complex.h at the command line
  extern type fftw_complex = 2*real(64); // 4.1.1
  extern type fftw_plan; // opaque type

  // Planner functions
  // Complex : 4.3.1
  // NOTE : We pass in arrays using ref 
  proc plan_dft(in1 : [] fftw_complex, out1 : [] fftw_complex, sign : c_int, flags :c_uint) : fftw_plan
  {
    param rank = in1.rank: c_int;

    var dims: rank*c_int;
    for param i in 1..rank do
      dims(i) = in1.domain.dim(i).size: c_int;

    extern proc fftw_plan_dft(rank: c_int, 
        n,  // BLC: having trouble being specific
        in1: [] fftw_complex, 
        out1: [] fftw_complex, 
        sign : c_int, flags : c_uint) : fftw_plan;

    return fftw_plan_dft(rank, dims, in1, out1, sign, flags);
  }

  // Real-to-complex and complex-to-real planning routines
  // There are two cases that we treat independently here : in-place and out-of-place transforms

  // Since the calls to FFTW are the same, pull the extern declarations out.
  // TODO : This should be cleaned up further and made consistent across the file
  extern proc fftw_plan_dft_r2c(rank: c_int, 
      n,  // BLC: having trouble being specific
      in1: [],
      out1: [], 
      flags : c_uint) : fftw_plan;
  extern proc fftw_plan_dft_c2r(rank: c_int, 
      n,  // BLC: having trouble being specific
      in1: [],
      out1: [],
      flags : c_uint) : fftw_plan;


  // Out-of-place routines
  proc plan_dft_r2c(in1 : [] real(64), out1 : [] fftw_complex, flags :c_uint) : fftw_plan
  {
    param rank = in1.rank: c_int;

    var dims: rank*c_int;
    for param i in 1..rank do
      dims(i) = in1.domain.dim(i).size: c_int;

    return fftw_plan_dft_r2c(rank, dims, in1, out1, flags);
  }
  proc plan_dft_c2r(in1 : [] fftw_complex, out1 : [] real(64),  flags :c_uint) : fftw_plan
  {
    param rank = out1.rank: c_int; // The dimensions are that of the real array

    var dims: rank*c_int;
    for param i in 1..rank do
      dims(i) = out1.domain.dim(i).size: c_int;

    return fftw_plan_dft_c2r(rank, dims, in1, out1, flags);
  }

  // In-place routines, note that these take in the true leading dimension
  // TODO : We should put in checks to see that the sizes are consistent
  // TODO : We should check on types...
  proc plan_dft_r2c(realDom : domain, in1 : [] ?t, flags : c_uint) : fftw_plan 
  {
    param rank = realDom.rank: c_int;

    var dims: rank*c_int;
    for param i in 1..rank do
      dims(i) = realDom.dim(i).size: c_int;

    return fftw_plan_dft_r2c(rank, dims, in1, in1, flags);
  }
  proc plan_dft_c2r(realDom : domain, in1: [] ?t, flags : c_uint) : fftw_plan 
  {
    param rank = realDom.rank: c_int;

    var dims: rank*c_int;
    for param i in 1..rank do
      dims(i) = realDom.dim(i).size: c_int;

    return fftw_plan_dft_c2r(rank, dims, in1, in1, flags);
  }


  // Using plans 
  proc execute(const plan : fftw_plan) {
    extern proc fftw_execute(const plan : fftw_plan);
    fftw_execute(plan);
  }
  proc destroy_plan(plan : fftw_plan) {
    extern proc fftw_destroy_plan(plan : fftw_plan);
    fftw_destroy_plan(plan);
  }
  proc cleanup() {
    extern proc fftw_cleanup();
    fftw_cleanup();
  }


  // Multi-threaded setup routines below
  proc init_threads() : c_int {
    extern proc fftw_init_threads() : c_int;
    return fftw_init_threads();
  }
  proc cleanup_threads() {
    extern proc fftw_cleanup_threads();
    fftw_cleanup_threads();
  }
  proc plan_with_nthreads(n : c_int) {
    extern proc fftw_plan_with_nthreads(n : c_int);
    fftw_plan_with_nthreads(n);
  }


  // Utilities -- not in FFTW
  proc abs((r,c) : fftw_complex) : real(64) {
    return sqrt(r*r + c*c);
  }

  proc re((r,c) : fftw_complex) : real(64) {
    return r;
  }
  proc im((r,c) : fftw_complex) : real(64) {
    return c;
  }


}
