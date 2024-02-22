proc main() {
  use M;

  foo();
}

module M {
  proc foo() {
    writeln("In M.foo()");
  }
}
