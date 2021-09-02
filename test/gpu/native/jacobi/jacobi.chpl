config const nSteps = 10;
config const n = 10;

writeln("on GPU:");
jacobi(here.getChild(1));
writeln("on CPU:");
jacobi(here);

proc jacobi(loc) {
  on loc {
    var A, B: [-1..n+1] real;

    A[-1] = 1; A[n+1] = 1;
    forall i in 0..n { A[i] = i:real; }

    for step in 1..nSteps {
      forall i in 0..n { B[i] = 0.33333 * (A[i-1] + A[i] + A[i+1]); }
      forall i in 0..n { A[i] = 0.33333 * (B[i-1] + B[i] + B[i+1]); }
    }
    writeln(A);
  }
}
