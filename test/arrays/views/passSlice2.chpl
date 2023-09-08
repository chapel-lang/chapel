const D = {1..9, 1..9};
var A: [D] real;

forall (i,j) in D with (ref A) do
  A[i,j] = i + j/10.0;

printArray(A[5..6, 7..8]);
writeln(A, "\n");
printArray2(A[5..6, 7..8]);
writeln(A, "\n");
printArray3(A[5..6, 7..8]);
writeln(A, "\n");
printArray4(A[5..6, 7..8]);
writeln(A, "\n");
printArray5(A[5..6, 7..8]);
writeln(A, "\n");

proc printArray(ref X) {
  forall x in X do
    x = 0.0;
}

proc printArray2(ref X: []) {
  forall i in X.domain with (ref X) do
    X[i] = 1.0;
}

proc printArray3(ref X: [] real) {
  for x in X do
    x = 2.0;
}

proc printArray4(ref X: [5..6, 7..8] real) {
  X = 3.0;
}

proc printArray5(ref X: [?D] ?t) {
  X = 4.0;
}
