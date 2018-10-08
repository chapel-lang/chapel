proc main() {
  var A: [1..10] int;
  var B = test(A, A);
}

// Alias analysis can prove a does not alias
// b because in intent is unique.
proc test(in a, in b) {
  var Result = a;
  for i in 1..100 {
    ref aref = a[1];
    ref bref = b[1];
    aref += bref;
  }
  return Result;
}
