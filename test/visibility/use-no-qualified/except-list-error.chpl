module A {
  var x: int;
  proc y() {
    writeln("In A.y()");
  }
}
module User {
  use A as _ except y;

  proc main() {
    writeln(A.x);
  }
}
