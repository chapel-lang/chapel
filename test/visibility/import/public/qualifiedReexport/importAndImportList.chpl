// Tests behavior when a module is imported from two different paths using list
// of symbols
module A {
  var x: int = 4;
  var y: int = 5;
  proc foo() {
    writeln("In A.foo()");
  }
  proc bar() {
    writeln("In A.bar()");
  }
}
module B {
  public import A.{x, bar};
}

module C {
  public import A.{y, foo};
}

module D {
  use B, C;

  proc main() {
    writeln(B.x);
    B.bar();
    writeln(C.y);
    C.foo();
  }
}
