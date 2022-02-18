module A {
  proc f() { writeln("A.f"); }
  var exxy = "A";
}
module B {
  proc f() { writeln("B.f"); }
  var exxy = "B";
}

module CUseA {
  public use A;
  proc f() { writeln("C.f"); }
  var exxy = "C";
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

module CUseA_ImportA {
  public use CUseA;
  import A;
}

module Program {
  use CUseA;         // -> f refers to C.f
                     // -> x refers to C.x

  proc main() {
    writeln(exxy);
  }
}
