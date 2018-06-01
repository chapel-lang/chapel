
proc foo(ref D : domain(1), in A : [] int) {
  D = {1..D.size};
  writeln(A.domain == D);
}

proc main() {
  var D = {1..4};
  var A : [D] int;
  foo(D, A);
}
