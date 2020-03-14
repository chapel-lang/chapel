module A {
  var x: int;
  proc foo() {
    writeln("In A.foo()");
  }
}
module User {
  use A only x;
  import A.foo;

  proc main() {
    writeln(A.x);
    writeln(x);
    foo();
    A.foo();
  }
}
