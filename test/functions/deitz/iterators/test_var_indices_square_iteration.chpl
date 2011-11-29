var A, B: [1..3] int;

for (a, b) in [A, B] {
  a = b + 1;
  b = a + 1;
}

writeln((A, B));

A = 0; B = 0;

for a in A {
  for b in B {
    a = b + 1;
    b = a + 1;
  }
}

writeln((A, B));
