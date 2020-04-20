module A {
  module B {
    module C {
      proc foo() {
        writeln("In A.B.C.foo()");
      }

      var x: bool;
    }
  }
}

module User {
  import A.B;
  import B.C; // enabled by previous import statement

  proc main() {
    writeln(C.x);
    C.foo();
  }
}
