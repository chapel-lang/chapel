module A {
  var foo: bool;

  proc bar() {
    writeln("In A.bar()");
  }
}
module B {
  private import A;

  proc baz() {
    writeln("In B.baz()");
    writeln(A.foo);
    A.bar();
  }
}

module User {
  use B;
  proc main() {
    baz();
  }
}
