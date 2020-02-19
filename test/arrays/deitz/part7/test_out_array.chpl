proc foo(out B: [1..3] int) {
  for i in B.domain do
    B(i) = i;
}

// Test with locals
proc test() {
  var A: [1..3] int;
  writeln(A);
  foo(A);
  writeln(A);
}
test();

// Test with globals
var A: [1..3] int;
writeln(A);
foo(A);
writeln(A);
