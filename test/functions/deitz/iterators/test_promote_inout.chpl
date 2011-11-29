proc foo(inout x: int, y: int) {
  x = x + y;
}

var A, B: [1..3] int;

A = 1..3;
B = 4..6;

writeln((A, B));
foo(A, B);
writeln((A, B));
