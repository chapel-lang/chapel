module M {
  proc foo() {
    writeln("In foo");
  }
}

module N {
  use M except *;

  proc main() {
    M.foo();
  }
}
