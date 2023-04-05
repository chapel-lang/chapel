module A {
  proc f() { writeln("A.f"); }
}
module B {
  proc f() { writeln("B.f"); }
}

module UseB {
  public use B;
}

module UseA_UseUseB {
  public use A;
  public use UseB;
}

module Program {
  use UseA_UseUseB;

  proc main() {
    f();
  }
}
