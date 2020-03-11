module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;
  }
}

module User {
  import A.B as C;

  proc main() {
    writeln(C.x);
    C.foo();
  }
}
