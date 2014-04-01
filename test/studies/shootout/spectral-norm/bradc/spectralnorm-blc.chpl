/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   Contributed by Lydia Duncan & Albert Sidelnik, based on Sebastien
   Loisel's C version
*/

config const n = 500;

// Note:
// Matrix A is never actually stored, its contents are computed on the fly
// through calls to the method A.  This is part of the benchmark's
// required implementation.  For ease of reference, it will be referred to as
// simply "A" when used.  Similarly, At is A transposed.

proc main() {
  var tmp, u, v: [0..#n] real;  // Declare three vectors

  u = 1.0;                      // Initialize u to 1.0

  for 1..10 {                   // For 10 iterations...
    multiplyAtAv(u, tmp, v);    // ...compute v = A*u*At
    multiplyAtAv(v, tmp, u);    // ...and     u = A*v*At
  }

  writef("%.9dr\n", sqrt(+ reduce (u*v) / + reduce (v*v)));
}

//
// Multiply vector v by matrix A and then by matrix A transposed
//
proc multiplyAtAv(v, tmp, AtAv) {
  multiplyAv(v, tmp);
  multiplyAtv(tmp, AtAv);
}


//
// Multiply vector v by matrix A
//
proc multiplyAv(v: [?Dv], Av: [?DAv]) {
  forall i in DAv do
    // The use of a serial statement here and in multiplyAtv is to avoid
    // creating additional tasks for the reduction.  Traversing the outer
    // loop in parallel is more efficient than dividing the smaller
    // computations up between parallel tasks.
    serial do
      Av[i] = + reduce [j in Dv] (A[i,j] * v[j]);
}


//
// Multiply vector V by matrix A transposed
//
proc multiplyAtv(v: [?Dv], Atv: [?DAtv]) {
  forall i in DAtv do
    serial do
      Atv[i] = + reduce [j in Dv] (A[j,i] * v[j]);
}


//
// return element i,j of infinite matrix A
//
inline proc A(i, j) {
  return 1.0 / ((((i + j) * (i + j + 1)) / 2) + i + 1);
}
