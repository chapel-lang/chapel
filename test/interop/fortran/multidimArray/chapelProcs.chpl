use ISO_Fortran_binding;

proc toChapelArray(ref A: CFI_cdesc_t, param rank = 1) {
  assert(A.rank == rank);
  var D: domain(rank);
  var dims: rank*range;

  for i in 0..#rank {
    dims(i+1) = A.dim[i].lower_bound..A.dim[i].extent /* by A.dim[i].sm ? */;
  }

  D = {(...dims)};
  var AA: [D] int; // getChplType(A.type);
  return AA;
}

export proc chpl_library_init_ftn() {
  extern proc chpl_library_init(argc: c_int, argv: c_ptr(c_ptr(c_char)));
  var filename = c"fake";
  chpl_library_init(1, c_ptrTo(filename): c_ptr(c_ptr(c_char)));;
  chpl__init_chapelProcs();
}

proc CFI_cdesc_t.this(idx:int...?rank) ref {
  assert(this.rank == rank);
  var subscripts: [0..#rank] CFI_index_t;
  for param i in 1..rank {
    subscripts[i-1] = idx[i]: CFI_index_t;
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
/*
      var subscripts: [0..1] CFI_index_t;
      subscripts[0] = i: CFI_index_t;
      subscripts[1] = k: CFI_index_t;
      var x: c_ptr(real) = CFI_address(A, c_ptrTo(subscripts)): c_ptr(real);
      x.deref() = (i*100 + k):real;
*/
    }
  }
}
