const D = {1..2, 1..2};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

printArray(A.reindex(0..1, 0..1));
printArray2(A.reindex(0..1, 0..1));
printArray3(A.reindex(0..1, 0..1));
printArray4(A.reindex(0..1, 0..1));
printArray5(A.reindex(0..1, 0..1));

proc printArray(X) {
  writeln(X);
  writeln(X.domain);
  writeln();
}

proc printArray2(X: []) {
  writeln(X);
  writeln(X.domain);
  writeln();
}

proc printArray3(X: [] real) {
  writeln(X);
  writeln(X.domain);
  writeln();
}

proc printArray4(X: [0..1, 0..1] real) {
  writeln(X);
  writeln(X.domain);
  writeln();
}

proc printArray5(X: [?D] ?t) {
  writeln(X);
  writeln(D);
  writeln(t:string);
}