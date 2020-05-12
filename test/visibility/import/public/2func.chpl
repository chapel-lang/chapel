module A {
  proc foo(x: int, y: int) {
    writeln("A.foo ", x, y);
  }
  proc foo(x: int) {
    writeln("A.foo ", x);
  }
}
module B {
  public import A.foo;
}
module User {
  import B;

  proc main() {
    B.foo(10);
    B.foo(2, 4);
  }
}
