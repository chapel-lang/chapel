var A, B, C: [1..4] int = 1..4;

for i in 1..4 {
  B[i] = A[i] + A[i] * A[i]; // B = 2 2 6 12
  A = i;
  C[i] = A[i] + A[i] * A[i]; // C = 2 6 12 20
}

writeln(A); // A = 4 4 4 4
writeln(B); // B = 2 2 6 12
writeln(C); // C = 2 6 12 20
