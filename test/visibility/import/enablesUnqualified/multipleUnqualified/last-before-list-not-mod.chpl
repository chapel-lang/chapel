module A {
  class B {
    var x: int;
  }
}
module User {
  import A.B.{x}; // should fail

  proc main() {
    writeln(x);
  }
}
