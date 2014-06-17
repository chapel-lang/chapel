proc foo(a: [] real) {
  writeln("array of reals");
  writeln(a);
}

proc foo(a: [] int) {
  writeln("array of ints");
  writeln(a);
}

var A: [1..4, 1..3] real;

foo(A);

var B: [1..3, 1..4] int;

foo(B);
