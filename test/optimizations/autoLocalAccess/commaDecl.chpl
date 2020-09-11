use common;

var D = createDom({1..10});

var A, B: [D] int;

B = 10;

forall i in D {
  A[i] = B[i];
}

writeln(A);
