export proc chpl_library_init_ftn() {
  use CPtr;
use SysCTypes;

  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));
  var filename = c"fake";
  chpl_library_init(1, c_ptrTo(filename): c_ptr(c_ptr(c_char)));;
  chpl__init_chapelProcs();
}

export proc takesArray(A: [] real) {
  forall i in A.domain {
    A[i] = i*10;
  }
}
