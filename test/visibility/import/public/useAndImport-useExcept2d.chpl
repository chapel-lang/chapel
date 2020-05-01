// Tests behavior when a module is both imported and used from different paths
// when the use has an except list, ensuring that we still fail to access the
// specified symbols.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public import A;
}

module C {
  public use A except foo;
}

module D {
  use B, C;

  proc main() {
    foo(); // Won't work
  }
}
