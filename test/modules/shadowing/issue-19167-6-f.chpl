module A {
  proc f() { writeln("A.f"); }
}
module B {
  proc f() { writeln("B.f"); }
}

module UseA_ImportB {
  public use A;
  import B;
}

module Program {
  use UseA_ImportB;

  proc main() {
    f();
  }
}
