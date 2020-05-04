module A {
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
  }
}
