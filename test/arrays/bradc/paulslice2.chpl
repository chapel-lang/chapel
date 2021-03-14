
config const n = 16;

const radix = 4;

var A: [1..n] int = [i in 1..n] i;

writeln("A is: ", A);

for span in 1..n/radix {
  foo(A[1.. by span # radix]);
}

proc foo(B:[?D] int) {
  writeln("B is: ", B);
}
