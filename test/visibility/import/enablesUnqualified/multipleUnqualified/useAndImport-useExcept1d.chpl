// Tests behavior when a module is both imported and used from different paths
// when the use has an except list, ensuring that we can still access the
// remaining symbols and utilize qualified access.
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
  public use A except foo;
}

module D {
  use B, C;

  proc main() {
    writeln(x); // OK, brought in by import
    // foo(); // Won't work
    writeln(A.x); // error: A not brought in
    A.foo(); // error: A not brought in
  }
}
