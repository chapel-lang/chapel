module A {
  pragma "last resort"
  var x: int = 1;
}

module B {
  var x: int = 2;
}

module Program {
  use A, B;
  proc main() {
    writeln(A.x);
    writeln(B.x);
    writeln(x);
  }
}
