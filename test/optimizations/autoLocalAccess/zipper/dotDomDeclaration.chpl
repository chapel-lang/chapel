use BlockDist;

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall (i, loopIdx) in zip(D, 1..) {
    A[i] = B[i] + C[i] * loopIdx;
  }
  writeln(A);
}

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = B[i] + C[i] * loopIdx;
  }
  writeln(A);
}

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall (i, loopIdx) in zip(B.domain, 1..) {
    A[i] = B[i] + C[i] * loopIdx;
  }
  writeln(A);
}

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall (i, loopIdx) in zip(C.domain, 1..) {
    A[i] = B[i] + C[i] * loopIdx;
  }
  writeln(A);
}
