module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
  var x = "C";
}

module CUseA_UseA {
  public use CUseA;
  public use A;
}

module Program {
  use CUseA_UseA;
  
  proc main() {
    writeln(x);
  }
}
