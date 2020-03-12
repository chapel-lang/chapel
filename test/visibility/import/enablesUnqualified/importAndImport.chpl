// Tests behavior when a module is imported from two different paths
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  import A.x;
}

module C {
  import A.foo;
}

module D {
  use B, C;

  proc main() {
    writeln(A.x);
    A.foo();
  }
}
