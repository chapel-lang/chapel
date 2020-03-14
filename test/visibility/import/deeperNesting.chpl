module A {
  module B {
    module C {
      proc foo() {
        writeln("In A.B.C.foo()");
      }

      var x: int;
    }
  }
}

module User {
  import A.B.C;

  proc main() {
    writeln(C.x);
    C.foo();
  }
}
