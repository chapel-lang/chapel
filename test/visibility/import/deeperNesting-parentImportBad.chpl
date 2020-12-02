module A {
  module B {
    proc foo() {
      writeln("In A.B.C.foo()");
    }

    var x: int;

    module C {
      import B; // Shouldn't work, need to say A.B

      proc main() {
        writeln(B.x);
        B.foo();
      }
    }
  }
}
