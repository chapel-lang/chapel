module A {
  pragma "last resort"
  proc x : int { return 1; }
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
