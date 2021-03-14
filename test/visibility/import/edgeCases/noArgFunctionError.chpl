module A {
  proc foo(x: int) {
    writeln(x);
  }
}
module B {
  public import A.foo;
}
module User {
  import B;

  proc main() {
    B.foo(); // Note: no arguments, but A.foo requires an int
  }
}
