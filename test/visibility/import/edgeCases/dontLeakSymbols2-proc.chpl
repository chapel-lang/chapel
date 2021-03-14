module A {
  proc foo() {
    writeln("A.foo()");
  }
}
module B {
  public import A.foo;

  proc bar() {
    writeln("B.bar()");
  }
}
module C {
  public import B.bar;
  import B.foo;
}
module User {
  use C;
  proc main() {
    C.foo(); // Shouldn't work
    // we do know about C, and B does pretend it has an x, but C
    // doesn't bring that in
  }
}
