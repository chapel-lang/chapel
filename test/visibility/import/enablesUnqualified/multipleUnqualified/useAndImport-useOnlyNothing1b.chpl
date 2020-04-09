// Tests behavior when a module is both imported and used from different paths
// when the use has an only list, ensuring that we can still utilize qualified
// access.
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public import A.{x};
}

module C {
  public use A only;
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
