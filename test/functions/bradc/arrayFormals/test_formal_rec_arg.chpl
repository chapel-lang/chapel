const D = {1..3, 1..3};

var A: [D] real;
var B: [1..3, 1..3] real;
var C: [0..2, 0..2] real;

proc foo(X: [1..3, 1..3] real) {
  writeln("In foo!");
}

foo(A);
foo(B);
foo(C);
