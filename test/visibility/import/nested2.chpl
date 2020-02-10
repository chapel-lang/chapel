module A {
  module B {
    var x = 13;

    proc foo() {
      writeln("In B.foo()");
    }
  }
  import B; // Not supported yet

  proc main() {
    writeln(B.x);
    B.foo();
  }
}
