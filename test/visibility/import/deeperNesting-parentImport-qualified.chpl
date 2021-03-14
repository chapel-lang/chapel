module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;

    module C {
      import A.B;

      proc main() {
        B.foo(); // Works: import gets us to B and qualified naming to foo
      }
    }
  }
}
