module A {
  proc f() { writeln("A.f"); }
  var exxy = "A";
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
  var exxy = "C";
}

module Program {
  use CUseA;

  proc main() {
    writeln(exxy);
  }
}
