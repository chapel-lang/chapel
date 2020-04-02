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
  // import C.{x, foo}; -- I can't do this at the moment
  import C.x;
  import C.foo;
  proc main() {
    writeln(x);
    foo();
  }
}
