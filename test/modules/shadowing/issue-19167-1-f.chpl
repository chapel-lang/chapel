module A {
  proc f() { writeln("A.f"); }
}
module B {
  proc f() { writeln("B.f"); }
}

module UseA_UseB {
  public use A;
  public use B;
}

module Program {
  use UseA_UseB;

  proc main() {
    f();
  }
}
