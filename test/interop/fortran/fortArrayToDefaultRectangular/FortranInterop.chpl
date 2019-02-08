use ISO_Fortran_binding;
use chapelProcs;

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
  for param i in 1..rank {
    subscripts[i-1] = idx[i]: CFI_index_t;
  }
  var x = CFI_address(this, c_ptrTo(subscripts)): c_ptr(real);
  return x.deref();
}

proc fortranArrayToChapelArray(ref FA: CFI_cdesc_t, param rank, type eltType) {

  assert(CFI_is_contiguous(FA) == 1);

  var cumulativeLength = 1;
  for param i in 1..rank {
    const eltSize = c_sizeof(eltType): int;
    assert(FA.dim[i-1].sm == cumulativeLength * eltSize);
    cumulativeLength *= FA.dim[i-1].extent;
  }

  var dims: rank*range;
  for param i in 1..rank {
    assert(FA.dim[i-1].lower_bound == 0);
    dims[i] = 1..#FA.dim[i-1].extent;
  }
  var D = {(...dims)};
  var A = D.buildArrayWith(eltType,
                          FA.base_addr: _ddata(eltType),
                          D.numIndices);
  A._unowned = true;
  return A;
}

