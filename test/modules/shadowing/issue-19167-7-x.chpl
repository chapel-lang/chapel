module A {
  proc f() { writeln("A.f"); }
  var x = "A";
}
module B {
  proc f() { writeln("B.f"); }
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
