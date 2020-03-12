// Tests behavior when a module is both imported and used from different paths
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
  public use A;
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
