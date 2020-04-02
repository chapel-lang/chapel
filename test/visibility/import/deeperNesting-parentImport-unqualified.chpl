module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;

    module C {
      import A.B;

      proc main() {
        foo(); // Should not work; import does not name foo and C doesn't see
               // B's symbols lexically.
      }
    }
  }
}
