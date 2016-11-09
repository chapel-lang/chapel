var A, B: [1..4] int = 1..4;

proc f(i) return i;

for (a, i) in zip(A, 1..4) {
  A[i] = A[i] * A[i];
  a = i;
  B[i] = A[i] + f(A[i]);
}

writeln(A);
writeln(B);
