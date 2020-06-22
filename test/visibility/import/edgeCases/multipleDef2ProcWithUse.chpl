module A {
  proc foo(x: int, y: int) {
    writeln("A.foo ", x, y);
  }
}
module B {
  proc foo(x: int) {
    writeln("B.foo ", x);
  }
}
module C {
  public use A;
  public use B;
}
module D {
  use C;

  proc main() {
    C.foo(10);
    C.B.foo(10);
    C.A.foo(1, 2);
  }
}

