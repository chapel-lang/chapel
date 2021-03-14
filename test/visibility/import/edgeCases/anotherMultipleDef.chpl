module A {
    proc foo() {
      writeln("Module scope A.foo()");
    }
}
module B {
  class A {
    proc type foo() {
      writeln("type method A.foo() for class A");
    }
  }
}

module Z {
  import A;
  use B;

  proc main() {
    A.foo();
  }
}
