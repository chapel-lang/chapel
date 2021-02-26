module A {
  var x: int;
  proc y() {
    writeln("In A.y()");
  }
}
module User {
  use A as _;

  proc main() {
    writeln(A.x); // Ensure we can't use qualified access on a symbol
  }
}
