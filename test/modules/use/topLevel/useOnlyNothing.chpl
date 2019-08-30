module M {
  proc foo() {
    writeln("In foo");
  }
}

module N {
  use M only;

  proc main() {
    M.foo();
  }
}
