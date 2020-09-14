use common;

{
  var D = createDom({1..10, 1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  B = 3;
  C = 7;

  // this is the very basic case and optimzed completely
  forall ((i,j), loopIdx) in zip(D, {1..10, 1..10}) {
    A[i,j] = B[i,j] + C[i,j] * loopIdx[0];
  }
  writeln(A);
}

{
  var D = createDom({1..10, 1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  B = 3;
  C = 7;

  // this only optimizes `A[i]` can do more static tracing to optimize others
  forall ((i,j), loopIdx) in zip(D, {1..10, 1..10}) {
    A[i,j] = B[i,j] + C[i,j] * loopIdx[0];
  }
  writeln(A);
}
