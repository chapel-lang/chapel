module A {
  var x = 2;
  var y = 3;
  proc foo() {
    writeln("A.foo");
  }
}
module B {
  var y = 4;
}
module C {
  public import A.{x as y, foo as bar};
}

module Z {
  use C, B;
  proc main() {
    writeln(y);
    bar();
  }
}
