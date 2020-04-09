config const n = 10;

proc kernel(A) {
  // This pattern comes up in LCALS
  ref A1 = A[1];
  ref A2 = A[2];

  for i in 1..n {
    A1[i] = A2[1] + A2[i];
  }
}

proc main() {
  var A:[1..10] [1..10] int;
  kernel(A);
}


