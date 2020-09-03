module A {
  var x = 2;
}
module B {
  var x = 4;
}
module C {
  public import A.x;
  // public use A; -- should have the same behavior
}
module D {
  public import C.x;
}

module Z {
  use D, B;
  proc main() {
    writeln(x);
  }
}
