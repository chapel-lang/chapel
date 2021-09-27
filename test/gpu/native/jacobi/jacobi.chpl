config const nSteps = 10;
config const n = 10;

/* We expect 21 kernel launches from the GPU jacobi call here. 1 for the
   initialization forall loop + (2 for the computation forall loops *
                                 10 for nSteps)
*/

writeln("on GPU:");
jacobi(here.getChild(1));
writeln("on CPU:");
jacobi(here);

proc jacobi(loc) {
  on loc {
    var A, B: [0..n+1] real;

    A[0] = 1; A[n+1] = 1;
    forall i in 1..n { A[i] = i:real; }

    for step in 1..nSteps {
      forall i in 1..n { B[i] = 0.33333 * (A[i-1] + A[i] + A[i+1]); }
      forall i in 1..n { A[i] = 0.33333 * (B[i-1] + B[i] + B[i+1]); }
    }
    writeln(A);
  }
}
