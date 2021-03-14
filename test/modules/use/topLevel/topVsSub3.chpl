module M {
  module NNNNN {
    proc foo() {
      writeln("In sub N.foo");
    }
  }

  proc main() {
    use NNNNN;
    foo();
  }
}
