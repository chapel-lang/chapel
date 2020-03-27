module A {
  module B {
    var x = 13;

    proc foo() {
      writeln("In B.foo()");
    }
  }
  import this.B;

  proc main() {
    writeln(B.x);
    B.foo();
  }
}
