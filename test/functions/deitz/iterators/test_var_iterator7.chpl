var A: [1..9] int;

for (a, i) in zip(A, 1..9) {
  a = i;
}

writeln("A = ", A);

