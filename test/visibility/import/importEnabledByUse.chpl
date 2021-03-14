// Tests when importing a module that was visible due to a use
module A {
  use B;
  import C; // Should work, because symbols in B have been brought in

  proc main() {
    writeln(C.x);
    C.foo();
  }
}

module B {
  module C {
    var x: bool;

    proc foo() {
      writeln("In B.C.foo()");
    }
  }
}
