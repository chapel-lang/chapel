module A {
  proc foo(x: int, y: int) {
    writeln("A.foo ", x, y);
  }
}
module B {
  proc bar(x: int) {
    writeln("B.bar ", x);
  }
}
module C {
  public import A.foo;
  public import B.bar as foo;
}
module D {
  import C;

  proc main() {
    C.foo(10);
    C.foo(2, 3);
  }
}
