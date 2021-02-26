module A {
  var x: int;
  proc foo() {
    writeln("In A.foo");
  }
}
module B {
  public import A;
}
module C {
  public import A;
}
module D {
  use B, C;
  // The following import should be possible and not problematic due to the
  // multiple public imports of A
  import A;

  proc main() {
    writeln(A.x);
    A.foo();
  }
}
