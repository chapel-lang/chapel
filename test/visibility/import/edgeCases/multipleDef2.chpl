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
  import C;

  proc main() {
    writeln(C.x); // Should be a conflict, which one should it pick?
  }
}
