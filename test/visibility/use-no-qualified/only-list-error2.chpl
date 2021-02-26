module A {
  var x: int;
  proc y() {
    writeln("In A.y()");
  }
}
module User {
  use A as _ only x;

  proc main() {
    A.y();
  }
}
