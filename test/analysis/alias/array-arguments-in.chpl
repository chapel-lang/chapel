config const n = 10;

proc main() {
  var A: [1..10] int;
  A = 1;
  var B = test(A, A);
  writeln(A);
  writeln(B);
}

// Alias analysis can prove a does not alias
// b because in intent is unique.
proc test(in a, in b) {
  var Result = a;
  for i in 1..n {
    ref aref = a[1];
    ref bref = b[1];
    aref += bref;
    Result[1] = aref;
  }
  return Result;
}
