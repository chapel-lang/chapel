// This code outlines the situation in:
//   test/library/packages/LinearAlgebra/correctness/no-dependencies/correctness.chpl
//
// Resolution follows these steps:
// * resolve the call to _expBySquaring() invoked from matPow()
// * create an instantiation of the generic proc _expBySquaring
//   call it GI ("generic instantiation")
// * store GI in genericsCache
// * start resolving the body of GI
// * resolve (find the target of) the recursive call to _expBySquaring()
// * the generic proc _expBySquaring is applicable,
//   check genericsCache for its instantiations
// * find GI, see if it is applicable
//   which requires that procs eye() and dot() are visible
// * yes, they are visible - from GI's POI#2 (proc main here),
//   via 'use MyLinearAlgebra.Sparse'
//
// If the compiler did not recognize that eye() and dot() were visible,
// it would get itself in trouble like so:
// * conclude that GI is not applicable
// * create a new instantiation of the generic _expBySquaring
//   call it GI-2
// * store GI-2 in genericsCache
// * make GI-2 the target of the call to _expBySquaring in GI
// * start resolving the body of GI-2
// * resolve (find the target of) the call to _expBySquaring() in GI-2
// * repeat the subsequent steps above for GI, this time for GI-2
// * create GI-3, then GI-4, etc. -- infinite recursion

module SuppressModuleWarning {

module correctness {  // correctness.chpl

  use super.MyLinearAlgebra;

  proc main {
    use MyLinearAlgebra.Sparse;
    var A = CSRMatrix(real);
    var B = matPow(A, 3);
    compilerError("done",0);
  }

}

module MyLinearAlgebra {

  proc matPow(A: [], b) {
    return _expBySquaring(A, b);
  }

  private proc _expBySquaring(x: ?t, n): t {
    eye();
    dot();
    return _expBySquaring(x, n);
  }

  module Sparse {

    proc CSRMatrix(type eltType) {
      return [1,2];
    }

    proc eye() { }
    proc dot() { }
  }
}

}
