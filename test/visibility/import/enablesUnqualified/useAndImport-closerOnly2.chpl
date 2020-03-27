module A {
  var x: int;
  proc foo() {
    writeln("In A.foo()");
  }
}
module User {
  import A.foo;
  use A only x;

  proc main() {
    writeln(A.x);
    writeln(x);
    foo();
    A.foo();
  }
}
