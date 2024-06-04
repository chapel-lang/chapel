const D = {1..3};

export proc foo(ref ret: [D] real, const ref input: [D] real) {
  writeln("In my exported routine");
  ret = 2 * input;
}

var A, B: [1..3] real;
A = 1..3;
foo(B, A);
writeln(B);
