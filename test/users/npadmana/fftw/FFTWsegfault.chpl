// Wrapper for FFTW version 3.
module FFTWsegfault {

  config param useWorkaround=false;

  // NOTE: See the three writeln's below which, when uncommented, make
  // the code work

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
  type _cxptr = c_ptr(2*c_double);
  type _rptr = c_ptr(c_double);

  // Planner functions
  // Complex : 4.3.1
  // NOTE : We pass in arrays using ref 
  proc plan_dft(dims, ref in1 : fftw_complex, ref out1 : fftw_complex, sign : c_int, flags :c_uint) : fftw_plan
    where (isHomogeneousTuple(dims))
    {
      extern proc fftw_plan_dft(rank : c_int, const ref n : c_int, in1 : _cxptr, out1 : _cxptr, sign : c_int, flags : c_uint) : fftw_plan;
      // Make sure types are correct
      param ndim : c_int = dims.size;
      var dims1 : ndim*c_int = dims;
      if useWorkaround then writeln(dims1);
      return fftw_plan_dft(ndim, dims1(1), c_ptrTo(in1) : _cxptr, c_ptrTo(out1) : _cxptr, sign, flags);
    }

  // Real-to-complex and complex-to-real plans
  // We handle these with a type parameter to let the user pass in an appropriately sized
  // real array for the complex part, most usually when doing an in-place transform.
  proc plan_dft_r2c(dims, ref in1 : ?t, ref out1 : ?tt, flags : c_uint) : fftw_plan 
    where ((t.type==real(64)) || (t.type==fftw_complex)) && ((tt.type==real(64)) || (tt.type==fftw_complex)) && (isHomogeneousTuple(dims))
    {
      //-- define the extern proc
      extern proc fftw_plan_dft_r2c(rank : c_int, const ref n : c_int,  in1 : _rptr,  out1 : _cxptr, flags : c_uint) : fftw_plan;
      // Make sure types are correct
      param ndim : c_int = dims.size;
      var dims1 : ndim*c_int = dims;
      if useWorkaround then writeln(dims1);
      return fftw_plan_dft_r2c(ndim,dims1(1),c_ptrTo(in1) : c_ptr(c_double), c_ptrTo(out1) : _cxptr, flags);
    }
  proc plan_dft_c2r(dims, ref in1 : ?t, ref out1 : ?tt, flags : c_uint) : fftw_plan 
    where ((t.type==real(64)) || (t.type==fftw_complex)) && ((tt.type==real(64)) || (tt.type==fftw_complex)) && (isHomogeneousTuple(dims))
    {
      //-- define the extern proc
      extern proc fftw_plan_dft_c2r(rank : c_int, const ref n : c_int, in1 : _cxptr,  out1 : _rptr, flags : c_uint) : fftw_plan;
      // Make sure types are correct
      param ndim : c_int = dims.size;
      var dims1 : ndim*c_int = dims;
      if useWorkaround then writeln(dims1);
      return fftw_plan_dft_c2r(ndim, dims1(1),c_ptrTo(in1) : _cxptr, c_ptrTo(out1) : c_ptr(c_double), flags);
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
    cleanup();
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
