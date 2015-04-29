/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   Contributed by Lydia Duncan & Albert Sidelnik, based on Sebastien
   Loisel's C version
*/

config const n = 500;

proc main() {
  var tmp, u, v: [0..#n] real;  // Declare three vectors
  const MatSpace = {0..#n, 0..#n};

  u = 1.0;                     // Initialize u to 1.0

  for 1..10 {                  // For 10 iterations...
    multiplyAtAv(u, tmp, v);   // ...compute v = A*u*At
    multiplyAtAv(v, tmp, u);   // ...and     u = A*v*At
  }

  writef("%.10r\n", sqrt(+ reduce (u*v) / + reduce (v*v)));
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
proc multiplyAv(v, Av) {
  Av = + reduce (dim = 2) [(i,j) in MatSpace] (A[i,j] * v[j]);
}


//
// Multiply vector V by matrix A transposed
//
proc multiplyAtv(v, Atv, MatSpace) {
  Atv = + reduce (dim = 2) [(i,j) in MatSpace] (Atv[j,i] * v[j]);
}


//
// return element i,j of infinite matrix A
//
inline proc A(i, j) {
  return 1.0 / ((((i + j) * (i + j + 1)) / 2) + i + 1);
}
