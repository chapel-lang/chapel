const n: uint(8) = 13;
const A: [0..<n, 0..<n] int = [(i,j) in {0..<n, 0..<n}] i*n + j;

writeln(A, "\n");

for i in 0..<n {
  const ref Ai = A[i, 0..<n];
  for j in 0..<n {
    write(Ai[j], " ");
  }
  writeln();
}
writeln();

for i in 0..<n do
  writeln("line ", i, " in A: ", A[i, 0..<n]);

