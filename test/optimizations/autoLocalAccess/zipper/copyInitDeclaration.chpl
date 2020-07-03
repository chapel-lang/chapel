use BlockDist;

proc foo() {
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C = newBlockArr({1..10}, int);

  B = 3;
  C = 7;

  // this is the very basic case and optimzed completely
  forall (i, loopIdx) in zip(D, 1..) {
    A[i] = B[i] + C[i] * loopIdx;
  }
  writeln(A);
}

//{
  //var D = newBlockDom({1..10});

  //var A: [D] real;
  //var B: [D] real;
  //var C: [D] real;

  //B = 3;
  //C = 7;

  //// this only optimizes `A[i]` can do more static tracing to optimize others
  //forall i in A.domain {
    //A[i] = B[i] + C[i];
  //}
  //writeln(A);
//}

foo();
