module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}
module B {
  proc f() { writeln("B.f"); }
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
    f();
  }
}
