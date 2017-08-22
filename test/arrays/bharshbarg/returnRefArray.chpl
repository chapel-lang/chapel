
proc helper(ref A) {
  for i in A.domain do
    A[i] += i;
  return A;
}

proc main() {
  var A : [1..10] int;

  for i in 1..10 do
    helper(A);

  writeln(A);
}
