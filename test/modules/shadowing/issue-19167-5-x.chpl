module A {
  var x = "A";
}

module CUseA {
  public use A;
  var x = "C";
}

module CUseA_ImportA {
  public use CUseA;
  import A;
}

module Program {
  use CUseA_ImportA;

  proc main() {
    writeln(x);
  }
}
