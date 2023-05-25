module A {
  proc f() { writeln("A.f"); }
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
}

module Program {
  use CUseA;

  proc main() {
    f();
  }
}
