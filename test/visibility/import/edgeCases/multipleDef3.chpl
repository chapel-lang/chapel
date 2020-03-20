module A {
  var x = 3;
}
module B {
  var x = 7;
}
module C {
  public import A.x;
  public import B.x;
}
module D {
  use C; // or import C for C.x

  proc main() {
    writeln(x); // Should be a conflict, which one should it pick?
  }
}
