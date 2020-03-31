module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;

    module C {
      import A.B; // Should work

      proc main() {
        writeln(B.x);
        B.foo();
      }
    }
  }
}
