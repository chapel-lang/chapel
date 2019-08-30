module M {
  proc foo() {
    writeln("In foo");
  }
}

module N {
  use M except foo;

  proc main() {
    M.foo();
  }
}
