// Tests behavior when a module is both imported and used from different paths
// when the use has an empty only list, ensuring that we can still utilize
// qualified access.
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
  public import A.{x};
}

module D {
  use B, C;

  proc main() {
    writeln(x);
    writeln(A.x);
    // foo(); // Also won't work
    A.foo();
  }
}
