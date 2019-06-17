module A {
  var first = 12;

  proc foo() {
    writeln("Hi, B!");
  }
}

module B {
  public use A;

  var second = -2;
}

module C {
  private use B;

  var third = second * 4;
}

module D {
  use C;

  proc main() {
    writeln(first);
    // Should fail.  Even though first is visible to B and C, C's use was
    // private, so any use of C shouldn't see symbols from A or B.
  }
}
