module A {
  var x = "A";
}
module B {
  var x = "B";
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
