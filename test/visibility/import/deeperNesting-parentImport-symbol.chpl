module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;

    module C {
      import A.B.foo;

      proc main() {
        foo(); // Should work: import gets us directly to foo()
      }
    }
  }
}
