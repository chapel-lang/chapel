use common;

var D = createDom({1..10});

var A, B: [D] int;

B = 10;

forall i in D with (ref A) {
  A[i] = B[i];
}

writeln(A);
