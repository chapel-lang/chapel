module A {
  var alone = 1;
}

module B {
  public use A;
  var thriceAlone = 3*alone;
}

module C {
  public use A;
  var twiceAlone = 2*alone;
}

module D {
  use B, C;

  proc main() {
    writeln(thriceAlone);
    writeln(twiceAlone);
    writeln(alone);
  }
}
