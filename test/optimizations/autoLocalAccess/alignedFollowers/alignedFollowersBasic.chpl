use common;

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  B = 3;
  C = 7;

  
  forall (elem, i) in zip(A, D) {
    elem = B[i] + C[i];
  }
  writeln(A);
}

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  B = 3;
  C = 7;

  // this only optimizes `A[i]` can do more static tracing to optimize others
  forall (elem, i) in zip(A, A.domain) {
    elem = B[i] + C[i];
  }
  writeln(A);
}
