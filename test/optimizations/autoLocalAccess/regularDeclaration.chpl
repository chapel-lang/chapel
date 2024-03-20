use common;

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  B = 3;
  C = 7;

  // this is the very basic case and optimzed completely
  forall i in D with (ref A) {
    A[i] = B[i] + C[i];
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


  forall i in A.domain with (ref A) {
    A[i] = B[i] + C[i];
  }
  writeln(A);
}
