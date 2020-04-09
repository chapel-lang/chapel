/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   a serial version derived from the Chapel version by Lydia Duncan,
   Albert Sidelnik, and Brad Chamberlain
*/

config const n = 500;           // the size of A (n x n), u and v (n-vectors)

proc main() {
  var tmp, u, v: [0..#n] real;

  u = 1.0;

  for 1..10 {
    multiplyAtAv(u, tmp, v);    // v = A^T*A*u
    multiplyAtAv(v, tmp, u);    // u = A^T*A*v
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
  for i in DAv {
    var av = 0.0;
    for j in Dv do
      av += A[i,j] * v[j];
    Av[i] = av;
  }
}

//
// Compute A-tranpose * v ('Atv').
//
proc multiplyAtv(v: [?Dv], Atv: [?DAtv]) {
  for i in DAtv {
    var atv = 0.0;
    for j in Dv do
      atv += A[j,i] * v[j];
    Atv[i] = atv;
  }
}

//
// Compute element i,j of the conceptually infinite matrix A.
//
inline proc A(i, j) {
  return 1.0 / ((((i+j) * (i+j+1)) / 2) + i + 1);
}
