export proc chpl_library_init_ftn() {
  use SysCTypes;

  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));
  var filename = c"fake";
  chpl_library_init(1, c_ptrTo(filename): c_ptr(c_ptr(c_char)));;
  chpl__init_chapelProcs();
}

/* Allow accessing the array using normal Chapel syntax. This uses the
   `CFI_address` function from the `ISO_Fortran_binding` module to get
   the address of element `idx`, then returns a dereference of that address.
   It assumes that the array contains `real(64)` values, but hopefully we
   can relax that assumption.

   A[i,j] = ...;
   var val = A[i,j];
 */
proc CFI_cdesc_t.this(idx:int...?rank) ref {
  assert(this.rank == rank);
  var subscripts: [0..#rank] CFI_index_t;
  for param i in 0..<rank {
    subscripts[i] = idx[i]: CFI_index_t;
  }
  var x = CFI_address(this, c_ptrTo(subscripts)): c_ptr(real);
  return x.deref();
}

export proc takesArray(ref A: CFI_cdesc_t) {

  assert(A.rank == 2);
  // Here 'A.type' is supposed to mean the field named type in the CFI_cdesc_t
  // record.  But 'type' is a keyword in Chapel so that field is commented out.
  //assert(A.type == CFI_type_double);

  for k in 0..#A.dim[1].extent {
    for i in 0..#A.dim[0].extent {
      A[i,k] = (i*100 + k):real;
    }
  }
}
