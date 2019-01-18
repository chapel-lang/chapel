var A: [1..4] int = 1..4;
ref B = A;

for i in 1..4 {
  B[i] = A[i] + A[i] * A[i];
}

writeln(A);
writeln(B);
