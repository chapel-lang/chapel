module A {
  var x = "A";
}
module B {
  var x = "B";
}

module UseA_UseB {
  public use A;
  public use B;
}

module Program {
  use UseA_UseB;

  proc main() {
    writeln(x);
  }
}
