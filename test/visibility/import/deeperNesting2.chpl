module A {
  module B {
    module C {
      proc foo() {
        writeln("In A.B.C.foo()");
      }

      var x: int;
    }
  }
  // Tests behavior when part of the path is provided, but not the full path
  import B.C;

  proc main() {
    writeln(C.x);
    C.foo();
  }
}
