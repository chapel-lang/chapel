proc main() {
  var A: [1..10] int;

  test(A, A);
}

// a and b are passed the same array in the above
// call so they must be possible to alias.
proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
