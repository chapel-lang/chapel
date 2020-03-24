module M {
  proc foo() { return 42; }
  module N {
    writeln(foo());
    proc main() {
    }
  }
}
