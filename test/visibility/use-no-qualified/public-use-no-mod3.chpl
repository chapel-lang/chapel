module A {
  proc foo(x: int) {
    writeln("A.foo ", x);
  }
}
module C {
  public use A;
}
module D {
  use C;

  proc main() {
    C.foo(1);
    C.A.foo(2);
  }
}
