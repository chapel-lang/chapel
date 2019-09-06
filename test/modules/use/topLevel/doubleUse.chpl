module M {
  proc foo() {
    writeln("In foo");
  }
}

module N {
  use M only;
  use M only foo;
  proc main() {
    M.foo();
  }
}
