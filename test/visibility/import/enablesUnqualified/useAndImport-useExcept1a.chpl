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
  public use A except x;
}

module C {
  // Note: this import is private and thus does not impact the symbols available
  import A.x;
}

module D {
  use B, C;

  proc main() {
    // writeln(x); // Won't work
    writeln(A.x);
    foo();
    A.foo();
  }
}
