use common;

var D = createDom({1..10});

var A, B: [D] int;

B = 10;

forall (i, loopIdx) in zip(D, 1..) {
  A[i] = B[i]*loopIdx;
}

writeln(A);
