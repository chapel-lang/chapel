export proc chpl_library_init_ftn() {
  use CPtr;
use SysCTypes;

  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));
  var filename = c"fake";
  chpl_library_init(1, c_ptrTo(filename): c_ptr(c_ptr(c_char)));;
  chpl__init_chapelProcs();
}

export proc takesArray(ref FA: CFI_cdesc_t) {

  assert(FA.rank == 2);
  assert(FA.ctype == CFI_type_double);

  ref A = makeArrayFromFortranArray(FA, real, 2);

  forall (i,j) in A.domain {
    A[i,j] = i*100+j;
  }
}
