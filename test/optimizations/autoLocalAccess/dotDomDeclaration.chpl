use common;

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall i in D with (ref A) {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall i in A.domain with (ref A) {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall i in B.domain with (ref A) {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [A.domain] real;
  var C: [B.domain] real;

  B = 3;
  C = 7;

  // domain information is not directly available for B and C
  forall i in C.domain with (ref A) {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}
