proc f(A: [], B: []) {
  for i in A.domain {
    B[i] = 1;
    B[i] = A[i] + A[i] * A[i];
  }
}

var A: [1..4] int;
A = 3;
f(A, A);
writeln(A);
