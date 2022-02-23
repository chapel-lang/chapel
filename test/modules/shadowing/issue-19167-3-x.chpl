module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
  var x = "C";
}

module Program {
  use CUseA;

  proc main() {
    writeln(x);
  }
}
