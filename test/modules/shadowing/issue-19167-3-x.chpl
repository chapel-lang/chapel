module A {
  var x = "A";
}

module CUseA {
  public use A;
  var x = "C";
}

module Program {
  use CUseA;

  proc main() {
    writeln(x);
  }
}
