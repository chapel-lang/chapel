use Random;
use BLAS;
use blas_helpers;

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc main() {
  test_rotg();
  test_rotg();
  test_rotmg();
  test_rot();
  test_rotm();
  test_swap();
  test_scal();
  test_copy();
  test_axpy();
  test_dot();
  test_dotu_sub();
  test_dotc_sub();
  test_sdsdot();
  test_nrm2();
  test_asum();
  test_amax();
}

proc test_rotg() {
  test_rotg_helper(real(32));
  test_rotg_helper(real(64));
  test_rotg_helper(complex(64));
  test_rotg_helper(complex(128));
}

proc test_rotg_helper(type t) {
  var passed = 0,
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);
  var name = "%srotg".format(blasPrefix(t));

  // Simple test
  {

  rotg(ref a : ?eltType, ref b : eltType, ref c : eltType, ref s : eltType){
  // var err = 
  // trackErrors(name, err, ... )

  }

}

//ref a : ?eltType, ref b : eltType, ref c : eltType, ref s : eltType
proc test_rotmg() {}
//ref d1: ?eltType, ref d2: eltType, ref b1: eltType, b2: eltType, P: []eltType
proc test_rot() {}
//X: [?D] ?eltType, Y: [D] eltType, c: eltType, s: eltType,  incY: c_int = 1, incX: c_int = 1
proc test_rotm() {}
//X: [?D]?eltType,  Y: [D]eltType,  P: [D]eltType, incY: c_int = 1, incX: c_int = 1
proc test_swap() {}
//X: [?D]?eltType, Y: [D]eltType, incY: c_int = 1, incX: c_int = 1
proc test_scal() {}
//X: [?D]?eltType, alpha:eltType, incX: c_int = 1
proc test_copy()  {}
//X: [?D]?eltType, Y: [D]eltType, incY: c_int = 1, incX: c_int = 1
proc test_axpy() {}
//X: [?D]?eltType, Y: [D]eltType, alpha:eltType, incY: c_int = 1, incX: c_int = 1
proc test_dot() {}
//X: [?D]?eltType,  Y: [D]eltType, incY: c_int = 1, incX: c_int = 1
proc test_dotu_sub() {}
//X: [?D]?eltType,  Y: [D]eltType, ref dotu, incY: c_int = 1, incX: c_int = 1
proc test_dotc_sub() {}
//X: [?D]?eltType, Y: [D]eltType, ref dotc, incY: c_int = 1, incX: c_int = 1
proc test_sdsdot() {}
//X: [?D]?eltType, Y: [D]eltType, alpha: eltType, incY: c_int = 1,incX: c_int = 1
proc test_nrm2() {}
//X: [?D]?eltType, incX: c_int = 1
proc test_asum() {}
//X: [?D]?eltType, incX: c_int = 1
proc test_amax() {}
//X: [?D]?eltType, incX: c_int = 1
