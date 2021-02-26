proc foo(inout x: int, y: int) {
  x = x + y;
}

var aa = 1;
var bb = 4;
writeln((aa,bb));
foo(aa, bb);
writeln((aa,bb));

var A, B: [1..3] int;

A = 1..3;
B = 4..6;

writeln((A, B));
foo(A, B);
writeln((A, B));
