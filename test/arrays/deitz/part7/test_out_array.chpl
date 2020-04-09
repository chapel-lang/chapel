proc foo(out B: [1..3] int) {
  for i in B.domain do
    B(i) = i;
}

// Test with locals
proc test1() {
  var A: [1..3] int;
  writeln(A);
  foo(A);
  writeln(A);
}
test1();

// Test with globals
var A: [1..3] int;
writeln(A);
foo(A);
writeln(A);

proc bar(out B) {
  for i in B.domain do
    B(i) = i;
}

// Test with locals
proc test2() {
  var A: [1..3] int;
  writeln(A);
  bar(A);
  writeln(A);
}
test2();

// Test with globals
var AA: [1..3] int;
writeln(AA);
bar(AA);
writeln(AA);
