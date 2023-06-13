module A {
  proc f() { writeln("A.f"); }
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
}

module CUseA_ImportA {
  public use CUseA;
  import A;
}

module Program {
  use CUseA_ImportA;

  proc main() {
    f();
  }
}
