module A {
  var x = "A";
}
module B {
  var x = "B";
}

module UseB {
  public use B;
}

module UseA_UseUseB {
  public use A;
  public use UseB;
}

module Program {
  use UseA_UseUseB;

  proc main() {
    writeln(x);
  }
}
