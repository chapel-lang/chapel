module A {
  var x: int;
}
module B {
  proc foo() {
    writeln("In B.foo");
  }
}
module C {
  public import A, B.foo;
}
module User {
  use C;

  proc main() {
    writeln(A.x);
    foo();
  }
}
