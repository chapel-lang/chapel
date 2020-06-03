module A {
  proc foo() {
    writeln("In A.foo()!");
  }
}

module User {
  proc main() {
    import A;

    A.foo();
  }
}
