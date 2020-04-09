proc main() {
  var A : [1..2, 1..2] [1..10] int;
  coforall a in A {
    a = 1;
  }
  for i in A.domain do writeln(i, ": ", A[i]);
}
