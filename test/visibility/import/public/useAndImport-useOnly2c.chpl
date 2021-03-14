// Tests behavior when a module is both imported and used from different paths
// when the use has an only list, ensuring that we can't still access the
// unspecified symbol.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public use A only x;
}

module C {
  public import A;
}

module D {
  use B, C;

  proc main() {
    foo(); // Won't work
  }
}
