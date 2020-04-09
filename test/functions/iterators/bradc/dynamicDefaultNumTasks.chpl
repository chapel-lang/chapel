use DynamicIters;

var A: [1..100] real;

forall i in dynamic(1..100, 8) {
  A[i] = i;
}

writeln(A);