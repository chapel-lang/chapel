// Tests behavior when a module is both imported and used from different paths
// when the use has an except list, ensuring that we can't still access the
// specified symbol.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public use A except x;
}

module C {
  public import A.{foo};
}

module D {
  use B, C;

  proc main() {
    writeln(x); // Won't work
  }
}
