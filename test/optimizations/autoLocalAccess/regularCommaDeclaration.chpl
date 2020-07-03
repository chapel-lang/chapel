use BlockDist;

{
  var D = newBlockDom({1..10});

  var A, B, C: [D] real;

  B = 3;
  C = 7;

  // this is the very basic case and optimzed completely
  forall i in D {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}

{
  var D = newBlockDom({1..10});

  var A, B, C: [D] real;

  B = 3;
  C = 7;

  // this only optimizes `A[i]` can do more static tracing to optimize others
  forall i in A.domain {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}
