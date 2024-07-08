module A {
  proc f() { writeln("A.f"); }
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
}

module CUseA_UseA {
  public use CUseA;
  public use A;
}

module Program {
  use CUseA_UseA;

  proc main() {
    f();
  }
}
