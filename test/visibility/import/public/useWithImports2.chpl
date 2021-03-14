module A {
  var x: int;
  proc foo() {
    writeln("A.foo");
  }
}

module B {
  public import A;

  var y: bool;
  proc bar() {
    writeln("B.bar");
    A.foo();
  }
}

module C {
  use B;

  proc main() {
    writeln(y);
    bar();
    A.foo();
  }
}
