module A {
  var x: int;
  proc foo() {
    writeln("In A.foo()");
  }
}
module User {
  use A except x;
  import A.{x};

  proc main() {
    writeln(A.x);
    writeln(x);
    foo();
    A.foo();
  }
}
