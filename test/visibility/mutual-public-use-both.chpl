// Test mutual public use with functions in both modules
// This used to cause duplicates where a module re-exported its own functions

module A {
  public use B;
  proc fooA() {
    writeln("A.fooA");
  }
}

module B {
  public use A;
  proc fooB() {
    writeln("B.fooB");
  }
}

module C {
  import A, B;

  proc main() {
    A.fooA();
    A.fooB();
    B.fooA();
    B.fooB();
  }
}
