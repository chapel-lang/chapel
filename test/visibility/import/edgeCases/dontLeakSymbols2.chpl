module A {
  var x: int;
}
module B {
  public import A.x;

  var y: bool;
}
module C {
  public import B.y;
  import B.x;
}
module User {
  use C;
  proc main() {
    writeln(C.x); // Shouldn't work
    // we do know about C, and B does pretend it has an x, but C
    // doesn't bring that in
  }
}
