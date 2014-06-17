var A: [1..3] int;

sync {
  for e in A {
    begin {
      e = 2;
    }
  }
}

writeln(A);
