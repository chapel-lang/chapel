var A: [1..3] int;

sync {
  for e in A {
    begin ref(e) {
      e = 2;
    }
  }
}

writeln(A);
