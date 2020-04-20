// Tests behavior when a module is both imported and used from different paths
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  import A;
}

module C {
  import A;
}

module D {
  use B, C;

  proc main() {
    foo();
  }
}
