// Tests behavior when a module is both imported and used from different paths
// when the use has an empty only list, ensuring that we can't still access the
// specified symbol.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public use A only;
}

module C {
  // Note: this import is private and thus does not impact the symbols available
  import A.{foo};
}

module D {
  use B, C;

  proc main() {
    foo(); // Won't work
  }
}
