
config const n = 16;

const radix = 4;

var A: [i in 1..n] int = i;

writeln("A is: ", A);

for span in 1..n/radix {
  foo(A[1 + (0..#radix)*span]);
}

def foo(B:[1..4] int) {
  writeln("B is: ", B);
}
