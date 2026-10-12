// Test mutual public use with function in one module
// This used to cause duplicate re-exports and ambiguous call errors

module A {
  public use B;
  proc foo() {
    writeln("A.foo");
  }
}

module B {
  public use A;
}

module C {
  import B;

  proc main() {
    B.foo();
  }
}
