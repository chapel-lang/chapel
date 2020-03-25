var A, B: [1..10] real;

forall i in 1..10 {
  ref a = A[i];
  i = i+1;
  a = i;
}

forall i in {1..10} {
  ref b = B[i];
  i = i+1;
  b = i;
}

writeln(A);
writeln(B);
