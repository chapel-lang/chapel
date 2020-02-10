module A {
  module B {
    var x = 13;

    proc foo() {
      writeln("In A.B.foo()");
    }
  }
}

module C {
  import A.B; // Not supported yet

  proc main() {
    //writeln(A.B.x);
    //writeln(B.x);
    //A.B.foo();
    //B.foo();
  }
}
