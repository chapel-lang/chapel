module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}
module B {
  proc f() { writeln("B.f"); }
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
