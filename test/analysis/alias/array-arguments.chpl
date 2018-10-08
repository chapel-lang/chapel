proc main() {
  var A: [1..10] int;
  var B: [1..10] int;

  test(A, B);
}

// Alias analysis can prove a does not alias
// b if it's always passed a different array.
proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
