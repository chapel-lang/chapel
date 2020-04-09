var A: [1..3] int;

sync {
  for e in A {
    begin with (ref e) {
      e = 2;
    }
  }
}

writeln(A);
