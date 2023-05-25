module A {
  var x = "A";
}

module CUseA {
  public use A;
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
