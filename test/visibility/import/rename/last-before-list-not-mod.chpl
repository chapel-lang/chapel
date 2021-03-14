module A {
  class B {
    var x: int;
  }
}
module User {
  import A.B.{x as y}; // should fail, renaming shouldn't hide the error message

  proc main() {
    writeln(y);
  }
}
