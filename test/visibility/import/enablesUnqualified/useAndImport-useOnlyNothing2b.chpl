// Tests behavior when a module is both imported and used from different paths
// when the use has an empty only list, ensuring that we can't still access the
// unspecified symbol.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  // Note: this import is private and thus does not impact the symbols available
  import A.x;
}

module C {
  public use A only;
}

module D {
  use B, C;

  proc main() {
    writeln(x); // Won't work
  }
}
