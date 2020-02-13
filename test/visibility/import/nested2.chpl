module A {
  module B {
    var x = 13;

    proc foo() {
      writeln("In B.foo()");
    }
  }
  import B; // How to write this so that the below will work?

  proc main() {
    writeln(B.x);
    B.foo();
  }
}
