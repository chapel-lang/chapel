config param shouldError = false;

if !shouldError {
  var A: [1..5] int;
  const B: [1..5] int = 1;

  A[2..3] = B[2..3];
  writeln(A);
}
else {
  const A: [1..5] int;
  const B: [1..5] int = 1;

  A[2..3] = B[2..3];
  writeln(A);
}
