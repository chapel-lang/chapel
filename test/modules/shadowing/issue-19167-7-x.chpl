module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}
module B {
  proc f() { writeln("B.f"); }
  var x = "B";
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
  var x = "C";
}

module UseA_UseB {
  public use A;
  public use B;
}

module UseB {
  public use B;
}

module UseA_UseUseB {
  public use A;
  public use UseB;
}

module CUseA_UseA {
  public use CUseA;
  public use A;
}

module UseA_ImportB {
  public use A;
  import B;
}

module Program {
  use UseA_ImportB;
  proc main() {
    writeln(x);
  }
}
