proc main() {
  var A: [1..10] real;
  forall i in 1..10 do
    A[i] = i;

  const ref B = A[1..3];
  begin with (in B)
    foo(B);
}

proc foo(X: [?D]) {
  X[2] = - X[2];
  writeln(X);
}
