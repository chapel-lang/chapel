/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Lydia Duncan, Albert Sidelnik, and Brad Chamberlain
   derived from the C version by Sebastien Loisel
*/

config const n = 500;           // the size of A (n x n), u and v (n-vectors)

proc main() {
  var tmp, u, v: [0..#n] real;

  u = 1.0;

  for 1..10 {                   // For 10 iterations...
    multiplyAtAv(u, tmp, v);    // ...compute     v = A^T*A*u
    multiplyAtAv(v, tmp, u);    // ...followed by u = A^T*A*v
  }

  writef("%.9dr\n", sqrt(+ reduce (u*v) / + reduce (v*v)));
}

//
// Compute A-transpose * A * v ('AtAv').
//
proc multiplyAtAv(v, tmp, AtAv) {
  multiplyAv(v, tmp);
  multiplyAtv(tmp, AtAv);
}

//
// Compute A * v ('Av').
//
proc multiplyAv(v: [?Dv], Av: [?DAv]) {
  forall i in DAv do
    Av[i] = + reduce (for j in Dv do A[i,j] * v[j]);
}

//
// Compute A-tranpose * v ('Atv').
//
proc multiplyAtv(v: [?Dv], Atv: [?DAtv]) {
  forall i in DAtv do
    Atv[i] = + reduce (for j in Dv do A[j,i] * v[j]);
}

//
// Compute element i,j of the conceptually infinite matrix A.
//
inline proc A(i, j) {
  return 1.0 / ((((i + j) * (i + j + 1)) / 2) + i + 1);
}
