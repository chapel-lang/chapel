// Tests behavior when a module is imported from two different paths
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  public import A.x;
}

module C {
  public import A.foo;
}

module D {
  use B, C;

  proc main() {
    writeln(B.x);
    C.foo();
  }
}
