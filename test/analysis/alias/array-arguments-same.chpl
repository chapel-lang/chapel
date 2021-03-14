config const n = 10;

proc main() {
  var A: [1..10] int = 1;

  test(A, A);
  writeln(A);
}

// a and b are passed the same array in the above
// call so they must be possible to alias.
proc test(a, b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
