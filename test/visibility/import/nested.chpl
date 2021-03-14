module A {
  module B {
    var x = 13;

    proc foo() {
      writeln("In A.B.foo()");
    }
  }
}

module C {
  import A.B;

  proc main() {
    writeln(B.x);
    B.foo();
  }
}
