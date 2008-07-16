/*
 *
 *  Matrix and Vector Norms Example
 *
 *  This example defines a module, Norm for computing matrix and
 *  vector norms, and a second module, TestNorm that tests the
 *  Norm module for 1D and 2D arrays:
 *
 *  The Norm module defines:
 *  - 1-norm, 2-norm, infinity norm and Frobenius norm for vectors
 *  - 1-norm, infinity norm and Frobenius norm for matrices
 * 
 *  The normType argument for the first few overloads of norm() is an
 *  enumeration indicating the norm to compute.

 * The norm function may also be called without specifying normType,
 * in which case the default norm for that array rank will be used.
 *
 */


module Norm {

  // normType specifies the type of norm to compute
  enum normType {norm1, norm2, normInf, normFrob};

  // vector norms
  def norm(x: [], p: normType) where x.rank == 1 {
    select (p) {
      when normType.norm1 do return + reduce abs(x);
      when normType.norm2 do return sqrt(+ reduce (abs(x)*abs(x)));
      when normType.normInf do return max reduce abs(x);
      when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));
      otherwise halt("Unexpected norm type");
    }
  }

  // matrix norms
  def norm(x: [?D], p: normType) where x.rank == 2 {
    select (p) {
      when normType.norm1 do
        return max reduce [j in D.dim(2)] (+ reduce abs(x[D.dim(1), j]));

      when normType.norm2 do
        halt("Haven't implemented 2-norm for 2D arrays yet");

      when normType.normInf do
        return max reduce [i in D.dim(1)] (+ reduce abs(x[i, D.dim(2)]));

      when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));

      otherwise halt("Unexpected norm type");
    }
  }

  // this module doesn't implement norms for > 2D arrays, so generate
  // a compile-timem error if the user tries to call one
  def norm(x: [], p: normType) where x.rank > 2 {
    compilerError("Norms not implemented for array ranks > 2D");
  }

  //  default norms
  def norm(x: []) {
    select (x.rank) {
      when 1 do return norm(x, normType.norm2);
      when 2 do return norm(x, normType.normFrob);
      otherwise compilerError("Norms not implemented for array ranks > 2D");
    }
  }
}


