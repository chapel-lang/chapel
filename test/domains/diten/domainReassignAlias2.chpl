proc main {
  var D = {1..10};
  var A: [D] int = D;

  proc foo(X: [] int) {
    D = {1..10};
    writeln(X);
  }

  foo(A[D]);
}
