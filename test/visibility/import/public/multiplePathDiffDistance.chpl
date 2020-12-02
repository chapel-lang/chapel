module A {
  proc foo(x: int, y: int) {
    writeln("A.foo ", x, y);
  }
}
module B1 {
  proc foo(x: int) {
    writeln("B1.foo ", x);
  }
}
module B2 {
  public import B1.foo;
}
module C {
  public import A.foo;
  public import B2.foo;
}
module D {
  import C;

  proc main() {
    C.foo(10);
    C.foo(2, 3);
  }
}
