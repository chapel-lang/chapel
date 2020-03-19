module A {
  var x = 2;
}

module B {
  private use A;

  var y: bool;
}

module C {
  use B;

  proc main() {
    writeln(A.x);
  }
}
