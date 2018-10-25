config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int = 1;

  test(A, B);
  writeln(A);
}

// Alias analysis can prove a does not alias
// b if it's always passed a different array.
proc test(a, b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
