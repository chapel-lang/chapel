module A {
  module B {
    proc foo() {
      writeln("In A.B.foo()");
    }

    var x: int;

    module C {
      import A.B.foo;

      proc main() {
        foo(); // Should work, foo is still in scope to main()
      }
    }
  }
}
