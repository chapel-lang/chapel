
// Tests behavior when a module is imported from two different paths
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  var x: int = 2;
  proc foo() {
    writeln("In B.foo()");
  }
}
module C {
  public import A.x;
  public import B.{x as y};
}

module D {
  public import A.foo;
  public import B.{foo as bar};
}

module E {
  use C, D;

  proc main() {
    writeln(x);
    foo();
    writeln(y);
    bar();
  }
}
