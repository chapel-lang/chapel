module A {
  var x = 5;
}
module B {
  proc foo() {
    writeln("B.foo()");
  }
}
module C {
  public import A.x;
  public import B.foo;
}

module Z {
  import C.{x, foo}; // This used to not work, while individual forms did

  proc main() {
    C.foo();
  }
}
