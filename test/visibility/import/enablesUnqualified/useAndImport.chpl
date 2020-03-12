// Tests behavior when a module is both imported and used from different paths
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public use A;
}

module C {
  // Note: this import is private and thus does not impact the symbols available
  import A.x;
}

module D {
  use B, C;

  proc main() {
    writeln(x);
    writeln(A.x);
    foo();
    A.foo();
  }
}
