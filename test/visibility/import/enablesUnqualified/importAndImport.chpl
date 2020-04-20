// Tests behavior when a module is imported from two different paths
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
  // Note: this import is private and thus does not impact the symbols available
  import A.foo;
}

module D {
  use B, C;

  proc main() {
    writeln(A.x);
    A.foo();
  }
}
