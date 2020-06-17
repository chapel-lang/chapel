use BlockDist;

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall i in D {
    A[i] = B[i] + C[i];
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
  forall i in A.domain {
    A[i] = B[i] + C[i];
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
  forall i in B.domain {
    A[i] = B[i] + C[i];
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
  forall i in C.domain {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}
