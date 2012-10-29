proc foo(x) {
  x = {1..0};
}

const D = {1..3};

var A: [D] real;

writeln("A is: ", A);

foo(A.domain);

writeln("A is: ", A);
